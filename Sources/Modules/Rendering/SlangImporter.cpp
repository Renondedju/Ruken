#include "Rendering/SlangImporter.hpp"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

USING_RUKEN_NAMESPACE

SlangImporter::SlangImporter() noexcept:
	m_session {[&] {
		Slang::ComPtr<slang::ISession> session {};

		createGlobalSession(m_global_session.writeRef());

		m_target_create_info .format                   = SLANG_SPIRV;
		m_target_create_info .profile                  = m_global_session->findProfile("spirv_1_6");
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
		".slang"
		".slang-module"
	};
}

// TODO: Proper error handling / logging.
IOTask<RkVoid> SlangImporter::Import(FileHandle const& in_file) noexcept
{
	// ---
	std::vector<RkChar> data {};
	data.resize(in_file->GetFileSize());
	co_await in_file->Read(data.data(), FileCursor {
		.offset   = 0,
		.position = EFilePosition::Beginning
	}, data.size());

	// --- Acquiring the section lock & loading module.
	Slang::ComPtr<slang::IBlob>   diagnostics {};
	Slang::ComPtr<slang::IModule> module	  {};

	auto const session {co_await m_session};
	module   = session->loadModuleFromSourceString( // ! Requires sources, the call attempts to read from the
		in_file->Path.ToString().c_str(),		   //  ! underlying filesystem, bypassing the RkFilesystem module.
		in_file->Path.Filename  .c_str(),         //   ! Will fail if attempted on a packaged build or any kind of
		data.data(), diagnostics.writeRef()      //    ! exotic configuration without direct access to the sources.
	);

	// TODO: Might throw for a warning ?
	if (!module)
		throw Exception(std::format("Failed to load module '{}' : {}",
			in_file->Path.Filename.c_str(),
			static_cast<const char*>(diagnostics.get()->getBufferPointer()))
		);

	// --- Retrieving entry points
	std::vector<slang::IEntryPoint*>    entry_points    {static_cast<std::size_t>(module->getDefinedEntryPointCount()), nullptr};
	std::vector<slang::IComponentType*> component_types {entry_points.size() + 1, nullptr};
	for (int entry_point = 0; entry_point < module->getDefinedEntryPointCount(); entry_point++)
		module->getDefinedEntryPoint(entry_point, &entry_points[entry_point]);

	component_types.assign_range(entry_points);
	component_types.back() = module;

	// --- Linking
	Slang::ComPtr<slang::IComponentType> program     	{};
	Slang::ComPtr<slang::IComponentType> linked_program {};

	session->createCompositeComponentType(component_types.data(), component_types.size(), program.writeRef());
	program->link(linked_program.writeRef(), diagnostics.writeRef());

	// TODO: Might throw for a warning ?
	if (diagnostics)
		throw Exception(std::format("Failed to link module '{}' : {}",
			in_file->Path.Filename.c_str(),
			static_cast<const char*>(diagnostics.get()->getBufferPointer()))
		);

	// --- Writing code to disk
	Slang::ComPtr<slang::IBlob> kernel {};
	for (int entry_point = 0; entry_point < module->getDefinedEntryPointCount(); entry_point++)
	{
		linked_program->getEntryPointCode(entry_point, 0, kernel.writeRef(), diagnostics.writeRef());

	}
}
