#include "Resources/Assets/AssetImporter.hpp"

#include "Filesystem/Filesystem.hpp"
#include "JobSystem/Awaitables/Primitives/WhenAll.hpp"
#include "Resources/Resource.hpp"

USING_RUKEN_NAMESPACE

AssetImporter::AssetImporter(ServiceProvider& in_provider) noexcept:
	Service {in_provider, typeid(AssetImporter)}
{}

IOTask<RkVoid> AssetImporter::Import(FilesystemPath const& in_file_path) const noexcept
{
	// Look for a compatible importer
	std::filesystem::path const& extension {in_file_path.path.extension()};
	Importer*					 importer  {GetCompatibleImporter(extension)};

	if (!importer)
		throw Exception(std::format("There is no available importer for the '{}' file extension.", extension.generic_string()));

	Filesystem*  filesystem {m_service_provider.LocateService<Filesystem>()};
	RUKEN_ASSERT(filesystem, "Cannot import assets without a filesystem");

	auto const resources {co_await importer->Import(m_service_provider, filesystem->Open(in_file_path))};

	// For each extracted resource, writing it back to disk
	co_await ParallelForeach2(resources, [&](std::shared_ptr<Resource> const& in_resource) -> DynamicTask<RkVoid> {

		auto const data {in_resource->Serialize()};
		auto const file {filesystem->Open(FilesystemPath {
			.location = EFilesystemLocation::ImportedAssets,
			.path     = in_file_path.path / in_resource->subresource_name
		})};

		co_await file->Write(data.data(), FileCursor {
			.offset   = 0ULL,
			.position = EFilePosition::Beginning
		}, data.size());
	});
}

AssetImporter::Importer* AssetImporter::GetCompatibleImporter(std::filesystem::path const& in_extension) const
{
	for (auto const& current : m_importers)
		for (auto const& extension : current->SupportedExtensions())
			if (extension == in_extension)
				return current.get();

	return nullptr;
}
