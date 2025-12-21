#pragma once

#include "Resources/ResourceLoader.hpp"
#include "Resources/ResourceManager.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Loads spirv files into a ShaderModule
struct SpirvLoader final : ResourceLoader
{
	/// @copydoc ResourceLoader::SupportedExtensions
	std::vector<std::string_view> SupportedExtensions() const noexcept override;

	/// @copydoc ResourceLoader::Load
	IOTask<ResourcePtr<>> Load(LoadContext&& in_context) const override;
};


END_RUKEN_NAMESPACE
