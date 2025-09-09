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
	ResourceManifest*		 manifest   {nullptr};
	ResourceIdentifier const identifier {std::hash<FilePath>()(in_file_path)};

	{
		std::lock_guard lock(m_manifests_mutex);

		// 1 - Check if the handle already exists
		if (m_manifests.contains(identifier))
			return ResourceHandle<TResource>(m_manifests.at(identifier));

		manifest = &m_manifests[identifier];
	}

	// 2 - Start loading if not
	std::filesystem::path const& extension {in_file_path.path.extension()};
	ResourceLoader        const* loader    {GetCompatibleLoader(extension)};

	if (!loader)
		throw Exception(std::format("There is no available loader for the '{}' file extension.", extension.generic_string()));

	manifest->path.asset_file    = {};
	manifest->path.resource_file = in_file_path;

	Load(manifest, loader);

	return ResourceHandle<TResource>(*manifest);
}

template<CResource TResource>
ResourceHandle<TResource> ResourceManager::Provide(TResource&& in_resource, ResourceIdentifier const in_identifier) noexcept
{
	ResourceManifest* manifest {nullptr};

	{ // 1 - Get or create the manifest
		std::lock_guard lock(m_manifests_mutex);
		manifest = &m_manifests[in_identifier];
	}

	// 2 - "Load" the actual resource
	manifest->path.asset_file    = {};
	manifest->path.resource_file = {};
	manifest->resource_ptr.exchange(std::make_shared<TResource>(std::move(in_resource)), std::memory_order_relaxed);
	manifest->load_event  .Trigger();

	return ResourceHandle<TResource>(*manifest);
}

END_RUKEN_NAMESPACE
