#include "Resources/ResourceManager.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Filesystem/Filesystem.hpp"

USING_RUKEN_NAMESPACE

ResourceManager::ResourceManager(ServiceProvider& in_provider) noexcept:
	Service {in_provider, typeid(ResourceManager)}
{}

ResourceLoader* ResourceManager::GetCompatibleLoader(std::filesystem::path const& in_extension) const
{
	// TODO: Use unordered map ?
	std::string const in_str {in_extension.generic_string()};
	for (auto const& current : m_loaders)
		for (auto const& extension : current->SupportedExtensions())
			if (extension == in_str)
				return current.get();

	return nullptr;
}

IOTask<RkVoid> ResourceManager::Load(ResourceManifest* in_manifest, ResourceLoader const* in_loader) const noexcept
{
	Filesystem* filesystem {m_service_provider.LocateService<Filesystem>()};

	RUKEN_ASSERT(filesystem, "Cannot load resources without a filesystem.");

	FileHandle    const file(filesystem->Open(in_manifest->path.resource_file));
	std::vector<RkByte> data(file->GetFileSize());

	co_await file->Read(data.data(), {}, data.size());

	ResourcePtr const resource {co_await in_loader->Load(LoadContext {
		.services  = m_service_provider,
		.file_path = in_manifest->path.resource_file,
		.data      = std::move(data)
	})};

	in_manifest->resource_ptr.exchange(resource, std::memory_order_relaxed);
}
