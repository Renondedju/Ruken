#include "Resources/Assets/AssetImporter.hpp"

#include "Core/JobSystem/Awaitables/Primitives/ParallelForEach.hpp"
#include "Filesystem/Filesystem.hpp"
#include "Resources/Resource.hpp"

USING_RUKEN_NAMESPACE

AssetImporter::AssetImporter(ServiceProvider& in_provider) noexcept:
	Service {in_provider, typeid(AssetImporter)}
{}

IOTask<RkVoid> AssetImporter::Import(FilePath const& in_file_path) const noexcept
{
	// --- 1. Setup & safety
	std::filesystem::path const& extension  {in_file_path.path.extension()};
	Importer*					 importer   {GetCompatibleImporter(extension)};
	Filesystem*					 filesystem {m_service_provider.LocateService<Filesystem>()};

	RUKEN_ASSERT(filesystem, "Cannot import assets without a filesystem");

	if (!importer)
		throw Exception(std::format("There is no available importer for the '{}' file extension.", extension.generic_string()));

	// --- 2. Import
	ImportContext context {
		.services   = m_service_provider,
		.asset_file = filesystem->Open(in_file_path),
		.resources  = {} // To be filled by the importer
	};

	co_await importer->Import(context);

	// For each extracted resource, writing it back to disk
	co_await ParallelForeach(context.resources,
		[&](ImportContext::ResourceData const& in_resource) -> DynamicTask<RkVoid> {

		co_await filesystem
			->Open (in_resource.path)
			->Write(in_resource.data.data(), {}, in_resource.data.size());
	});
}

AssetImporter::Importer* AssetImporter::GetCompatibleImporter(std::filesystem::path const& in_extension) const
{
	std::string const in_str {in_extension.generic_string()};
	for (auto const& current : m_importers)
		for (auto const& extension : current->SupportedExtensions())
			if (extension == in_str)
				return current.get();

	return nullptr;
}
