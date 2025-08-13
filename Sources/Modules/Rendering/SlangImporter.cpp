#include "Rendering/SlangImporter.hpp"
#include "Rendering/SpirvModule.hpp"

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

std::vector<std::string> SlangImporter::SupportedExtensions() const noexcept
{
	return {
		".slang",
		".slang-module"
	};
}

// TODO: Proper error handling / logging.
IOTask<std::vector<std::shared_ptr<Resource>>> SlangImporter::Import(ServiceProvider const& in_services, FileHandle const& in_file) noexcept
{
	Slang::ComPtr<slang::IBlob> diagnostics {};
	Logger const*				logger		{in_services.LocateService<Logger>()};

	/** A simple debug helper. */
	auto check_result = [&](SlangResult const in_result) -> RkVoid
	{
		if (SLANG_FAILED(in_result))
			throw Exception(std::format("Failed to load module '{}' : {}",
				in_file->path.path.generic_string(),
				static_cast<const RkChar*>(diagnostics->getBufferPointer()))
			);

		if (diagnostics)
			logger->Warning(AssetImporter::service_name, "{}", static_cast<const RkChar*>(diagnostics->getBufferPointer()));

		diagnostics = nullptr;
	};

	// --- Reading the source file
	std::vector<RkChar> data {};
	data.resize(in_file->GetFileSize());
	co_await in_file->Read(data.data(), FileCursor {
		.offset   = 0,
		.position = EFilePosition::Beginning
	}, data.size());

	// --- Acquiring the section lock & loading module.
	Slang::ComPtr<slang::IModule> module {};

	auto const session {co_await m_session};
	module   = session->loadModuleFromSourceString(				 // ! Requires sources, the call attempts to read from the
		in_file->path.path			 .generic_string().c_str(), //  ! underlying filesystem, bypassing the RkFilesystem module.
		in_file->path.path.filename().generic_string().c_str(),//   ! Will fail if attempted on a packaged build or any kind of
		data.data(), diagnostics.writeRef()					  //    ! exotic configuration without direct access to the sources.
	);

	// TODO: Might throw for a warning ?
	if (!module)
		throw Exception(std::format("Failed to load module '{}' : {}",
			in_file->path.path.generic_string(),
			static_cast<const char*>(diagnostics.get()->getBufferPointer()))
		);

	// --- Retrieving entry points
	std::size_t const entry_point_count {static_cast<std::size_t>(module->getDefinedEntryPointCount())};

	std::vector<slang::IEntryPoint*>    entry_points   (entry_point_count       , nullptr);
	std::vector<slang::IComponentType*> component_types(entry_point_count + 1ULL, nullptr);
	for (std::size_t entry_point {}; entry_point < entry_point_count; entry_point++)
	{
		check_result(module->getDefinedEntryPoint(entry_point, &entry_points[entry_point]));
		component_types[entry_point] = entry_points[entry_point];
	}

	component_types[entry_points.size()] = module;

	// --- Linking
	Slang::ComPtr<slang::IComponentType> program     	  {};
	Slang::ComPtr<slang::IComponentType> linked_program   {};

	check_result(session->createCompositeComponentType(component_types.data(), component_types.size(), program.writeRef(), diagnostics.writeRef()));
	check_result(program->link(linked_program.writeRef(), diagnostics.writeRef()));

	// --- Generating resources
	std::vector<std::shared_ptr<Resource>> resources (entry_points.size());
	for (std::size_t entry_point = 0ULL; entry_point < entry_points.size(); entry_point++)
	{
		Slang::ComPtr<slang::IBlob> kernel {};
		check_result(linked_program->getEntryPointCode(entry_point, 0, kernel.writeRef(), diagnostics.writeRef()));

		std::string			name       {entry_points[entry_point]->getFunctionReflection()->getName()};
		std::vector<RkByte>	byte_array (kernel->getBufferSize());
		std::memmove(byte_array.data(), kernel->getBufferPointer(), kernel->getBufferSize());

		resources[entry_point] = std::make_shared<SpirvModule>(in_file->path, std::format("{}.spv", name), byte_array);
	}

	co_return resources;
}