#pragma once

#include "Resources/ResourceManager.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TLoader> requires std::is_base_of_v<ResourceLoader, TLoader>
RkVoid ResourceManager::ProvideLoader() noexcept
{
	m_loaders.emplace_back(std::make_unique<TLoader>());
}

template<CResource TResource>
ResourceHandle<TResource> ResourceManager::Request(FilePath const& in_file_path)
{
	ResourceManifest* manifest {nullptr};

	{
		std::lock_guard lock(m_manifests_mutex);

		// 1 - Check if the handle already exists
		if (m_manifests.contains(in_file_path))
			return ResourceHandle<TResource>(m_manifests.at(in_file_path));

		manifest = &m_manifests[in_file_path];
	}

	// 2 - Start loading if not
	std::filesystem::path const& extension {in_file_path.path.extension()};
	ResourceLoader        const* loader    {GetCompatibleLoader(extension)};

	if (!loader)
		throw Exception(std::format("There is no available loader for the '{}' file extension.", extension.generic_string()));

	manifest->path.asset_file    = {};
	manifest->path.resource_file = in_file_path;
	manifest->load_task			 = Load(manifest, loader);

	return ResourceHandle<TResource>(*manifest);
}

END_RUKEN_NAMESPACE