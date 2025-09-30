#pragma once

#include "Resources/ResourceManager.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TLoader> requires std::is_base_of_v<ResourceLoader, TLoader>
RkVoid ResourceManager::ProvideLoader() noexcept
{
	m_loaders.emplace_back(std::make_unique<TLoader>());
}

template<CResourceData TResource>
ResourceHandle<TResource> ResourceManager::Request(FilePath const& in_file_path)
{
	ResourceManifest* manifest {nullptr};
	std::size_t	const hash     {std::hash<FilePath>()(in_file_path)};

	auto deleter = [this, hash](ResourceManifest*) {
		std::lock_guard lock(m_manifests_mutex);
		m_manifests.erase(hash);
	};

	{
		std::lock_guard lock(m_manifests_mutex);

		// 1 - Check if the handle already exists
		if (m_manifests.contains(hash))
			return ResourceHandle<TResource>(std::shared_ptr<ResourceManifest>(&m_manifests.at(hash), deleter));

		manifest = &m_manifests[hash];
	}

	// 2 - Start loading if not
	std::filesystem::path const& extension {in_file_path.path.extension()};
	ResourceLoader        const* loader    {GetCompatibleLoader(extension)};

	if (!loader)
		throw Exception(std::format("There is no available loader for the '{}' file extension.", extension.generic_string()));

	Load(manifest, loader, in_file_path);

	return ResourceHandle<TResource>(std::shared_ptr<ResourceManifest>(manifest, deleter));
}

END_RUKEN_NAMESPACE
