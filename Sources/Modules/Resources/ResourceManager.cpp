#include "Resources/ResourceManager.hpp"

#include "Debug/Logging/Logger.hpp"
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

IOTask<RkVoid> ResourceManager::Load(ResourceManifest* in_manifest, ResourceLoader const* in_loader, FilePath in_filepath) const noexcept
{
	Filesystem* filesystem {m_service_provider.LocateService<Filesystem>()};
	RUKEN_ASSERT(filesystem, "Cannot load resources without a filesystem.");

	try
	{
		FileHandle    const file(filesystem->Open(in_filepath));
		std::vector<RkByte> data(file	   ->GetFileSize());

		co_await file->Read(data.data(), {}, data.size());

		ResourcePtr const resource {co_await in_loader->Load(LoadContext {
			.services  = m_service_provider,
			.file_path = in_filepath,
			.data      = std::move(data)
		})};

		in_manifest->resource_ptr.exchange(resource, std::memory_order_relaxed);
		in_manifest->load_event  .Trigger();
	}
	catch (...)
	{
		in_manifest->load_event  .Trigger(std::current_exception());

		if (auto const* logger {m_service_provider.LocateService<Logger>()})
			logger->Exception(service_name, "A resource could not be loaded.");
	}
}
