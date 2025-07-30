#pragma once

#include "Resources/ResourceImporter.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Transforms slang files into SPIR-V shader modules
struct SlangImporter final : ResourceImporter
{
	#pragma region Lifetime

	SlangImporter() noexcept					   = default;
	SlangImporter			(SlangImporter const&) = default;
	SlangImporter			(SlangImporter&&)      = default;
	SlangImporter& operator=(SlangImporter const&) = default;
	SlangImporter& operator=(SlangImporter&&)      = default;
	~SlangImporter() override					   = default;

	#pragma endregion

	#pragma region Methods

	/// @copydoc ResourceImporter::SupportedExtensions
	std::vector<std::string> SupportedExtensions() const noexcept override;

	/// @copydoc ResourceImporter::Import
	IOTask<RkVoid> Import(FileHandle const& in_file) noexcept override;

	#pragma endregion
};

END_RUKEN_NAMESPACE