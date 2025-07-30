#include "Rendering/SlangImporter.hpp"

#include <slang/slang-com-ptr.h>
#include <slang/slang.h>

USING_RUKEN_NAMESPACE

std::vector<std::string> SlangImporter::SupportedExtensions() const noexcept
{
	return {
		".slang"
	};
}

IOTask<RkVoid> SlangImporter::Import(FileHandle const& in_file) noexcept
{
	std::vector<RkChar> data {};
	data.resize(in_file->GetFileSize());

	// Reading the file
	co_await in_file->Read(data.data(), FileCursor {
		.offset   = 0,
		.position = EFilePosition::Beginning
	}, data.size());

	// Unfortunately slang does not properly support multithreading for now
	// so we have to create one instance per thread even though this is a bit wasteful.
	// Profiling is still needed.

	std::vector<slang::PreprocessorMacroDesc> macros   {};
	std::vector<slang::CompilerOptionEntry  > options  {
	    {slang::CompilerOptionName::EmitSpirvDirectly, {slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr}}
	};

	Slang::ComPtr<slang::IGlobalSession> global_session {};
	Slang::ComPtr<slang::ISession>       session        {};
	slang::SessionDesc                   session_desc   {};
	slang::TargetDesc                    target_desc    {};
	createGlobalSession(global_session.writeRef());

	target_desc .format      = SLANG_SPIRV;
	target_desc .profile     = global_session->findProfile("spirv_1_6");
	session_desc.targets     = &target_desc;
	session_desc.targetCount = 1;
	session_desc.preprocessorMacros       = macros .data();
	session_desc.preprocessorMacroCount   = macros .size();
	session_desc.compilerOptionEntries    = options.data();
	session_desc.compilerOptionEntryCount = options.size();

	global_session->createSession(session_desc, session.writeRef());

	Slang::ComPtr<slang::IBlob> diagnostics  {};
	Slang::ComPtr const		    slang_module {
		session->loadModuleFromSourceString(
			in_file->Path.Directory.ToString().c_str(),
			in_file->Path.Filename			  .c_str(), data.data(), diagnostics.writeRef())
	};

	if (!slang_module)
	{
		throw Exception(std::format("Failed to load module '{}' : {}",
			in_file->Path.Filename.c_str(),
			static_cast<const char*>(diagnostics.get()->getBufferPointer()))
		);
	}
}
