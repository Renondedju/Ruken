#pragma once

#include "Core/JobSystem/Awaitables/Primitives/Mutex.hpp"

#include "Resources/ResourceImporter.hpp"
#include "Rendering/SlangSearchPath.hpp"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

BEGIN_RUKEN_NAMESPACE

/// @brief Transforms slang files into SPIR-V shader modules
struct SlangImporter final : ResourceImporter
{
	#pragma region Lifetime

	SlangImporter() noexcept;
	SlangImporter			(SlangImporter const&) = delete;
	SlangImporter			(SlangImporter&&)      = delete;
	SlangImporter& operator=(SlangImporter const&) = delete;
	SlangImporter& operator=(SlangImporter&&)      = delete;
	~SlangImporter() override					   = default;

	#pragma endregion

	#pragma region Methods

	/// @copydoc ResourceImporter::SupportedExtensions
	std::vector<std::string> SupportedExtensions() const noexcept override;

	/// @copydoc ResourceImporter::Import
	IOTask<RkVoid> Import(FileHandle const& in_file) noexcept override;

	#pragma endregion

	private:

		#pragma region Members

		slang::SessionDesc m_session_create_info {};
		slang::TargetDesc  m_target_create_info  {};

		std::vector<const RkChar*>		    const m_search_paths {SlangSearchPath::s_path};
		std::vector<slang::PreprocessorMacroDesc> m_macros       {};
		std::vector<slang::CompilerOptionEntry  > m_options      {
			{slang::CompilerOptionName::EmitSpirvDirectly, {slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr}}
		};

		Slang::ComPtr<slang::IGlobalSession>  m_global_session {};
		Mutex<Slang::ComPtr<slang::ISession>> m_session;

		#pragma endregion
};

END_RUKEN_NAMESPACE