#include "Rendering/SlangImporter.hpp"
#include "Rendering/ShaderModule.hpp"

#include "Filesystem/Filesystem.hpp"
#include "Core/Debug/Logging/Logger.hpp"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

USING_RUKEN_NAMESPACE

SlangImporter::SlangImporter() noexcept:
	m_session {[&] {
		Slang::ComPtr<slang::ISession> session {};

		createGlobalSession(m_global_session.writeRef());

		m_target_create_info .format                   = SLANG_SPIRV;
		m_target_create_info .profile                  = m_global_session->findProfile("spirv_1_5");
		m_session_create_info.targets                  = &m_target_create_info;
		m_session_create_info.targetCount              = 1;
		m_session_create_info.searchPaths              = m_search_paths.data();
		m_session_create_info.searchPathCount          = static_cast<uint32_t>(m_search_paths.size());
		m_session_create_info.preprocessorMacros       = m_macros .data();
		m_session_create_info.preprocessorMacroCount   = static_cast<uint32_t>(m_macros .size());
		m_session_create_info.compilerOptionEntries    = m_options.data();
		m_session_create_info.compilerOptionEntryCount = static_cast<uint32_t>(m_options.size());

		m_global_session->createSession(m_session_create_info, session.writeRef());

		return session;
	}()}
{}

std::vector<std::string_view> SlangImporter::SupportedExtensions() const noexcept
{
	return {
		".slang",
		".slang-module"
	};
}

IOTask<RkVoid> SlangImporter::Import(ImportContext& in_context) noexcept
{
	Slang::ComPtr<slang::IBlob> diagnostics {};
	auto   const& path   {in_context.asset_file->path};
	Logger const* logger {in_context.services.LocateService<Logger>()};

	auto log_or_throw = [&](RkBool const in_success) {
		if (!in_success)
			throw Exception(std::format("Could not import file named '{}' : {}",
				path.ToString(), static_cast<const RkChar*>(diagnostics->getBufferPointer()))
			);

		// Success but still have some diagnostics
		if (diagnostics != nullptr)
			logger->Warning(AssetImporter::service_name,
				"Slang importer returned some diagnostics : {}", static_cast<const RkChar*>(diagnostics->getBufferPointer())
			);
	};

	// --- 1. Reading the source file
	std::vector<RkChar> data {};
	data.resize(in_context.asset_file->GetFileSize());
	co_await in_context.asset_file->Read(data.data(), FileCursor {
		.offset   = 0,
		.position = EFilePosition::Beginning
	}, data.size());

	// --- 2. Acquiring the section lock & loading module.
	Slang::ComPtr<slang::IModule> module {};

	auto const  session {co_await m_session};
	module   = session->loadModuleFromSourceString(      // ! Requires sources, the call attempts to read from the
		path.path		    .generic_string().c_str(),  //  ! underlying filesystem, bypassing the RkFilesystem module.
		path.path.filename().generic_string().c_str(), //   ! Will fail if attempted on a packaged build or any kind of
		data.data(), diagnostics.writeRef()	      //    ! exotic configuration without direct access to the sources.
	);

	log_or_throw(module != nullptr);

	// --- 3. Retrieving entry points
	std::size_t const entry_point_count {static_cast<std::size_t>(module->getDefinedEntryPointCount())};

	std::vector<slang::IEntryPoint*>    entry_points   (entry_point_count       , nullptr);
	std::vector<slang::IComponentType*> component_types(entry_point_count + 1ULL, nullptr);

	component_types[entry_points.size()] = module;
	for (std::size_t entry_point {}; entry_point < entry_point_count; entry_point++)
	{
		log_or_throw(SLANG_SUCCEEDED(
			module->getDefinedEntryPoint(entry_point, &entry_points[entry_point]))
		);

		component_types[entry_point] = entry_points[entry_point];
	}

	// --- 4. Linking
	Slang::ComPtr<slang::IComponentType> program     	  {};
	Slang::ComPtr<slang::IComponentType> linked_program   {};

	log_or_throw(SLANG_SUCCEEDED(
		session->createCompositeComponentType(component_types.data(), component_types.size(), program.writeRef(),
	diagnostics.writeRef())));

	log_or_throw(SLANG_SUCCEEDED(
		program->link(linked_program.writeRef(),
	diagnostics.writeRef())));

	// --- 5. Exporting code
	Slang::ComPtr<slang::IBlob> kernel {};

	log_or_throw(SLANG_SUCCEEDED(
		linked_program->getTargetCode(0, kernel.writeRef(),
	diagnostics.writeRef())));

	//std::string			name       {entry_points[entry_point]->getFunctionReflection()->getName()};
	std::vector<RkByte>	byte_array (kernel->getBufferSize());
	std::memmove(byte_array.data(), kernel->getBufferPointer(), kernel->getBufferSize());

	in_context.resources.emplace_back(ImportContext::ResourceData {
		.data = byte_array,
		.path = FilePath {
			.location = EFilesystemLocation::ImportedAssets,
			.path     = in_context.asset_file->path.path / "code.spv"
		 }
	});
}