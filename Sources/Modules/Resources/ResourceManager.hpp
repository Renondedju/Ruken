#pragma once

#include "Core/Meta/Meta.hpp"
#include "Core/Service.hpp"

#include "Resources/Assets/AssetImporter.hpp"
#include "Resources/ResourceManifest.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceLoader.hpp"
#include "Resources/Resource.hpp"

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/FilePath.hpp"

#include <unordered_map>

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Imports and manages resources lifetime.
 *
 * The resource pipeline starts from an asset file that we need to import.
 * Importing an asset allows us to extract multiple resources from it, as well as
 * applying some kind of pre-processing to optimize the runtime. (Ex.: A .fbx file containing multiple models).
 *		This process is usually done ahead of time, although you could make it happen at runtime with proper care.
 *		Importers are not (yet ?) required to use a Filesystem service to read an asset.
 *
 * Resulting artifacts are then serialized to the filesystem, ready to be picked up by the runtime.
 *
 * | ----------- AHEAD OF TIME ------------ | ------------ RUNTIME -------------- |
 * |                                        |                                     |
 *				   / Resource > Save \              / Resource > Load \
 * Asset > Import |> Resource > Save |> Filesystem |> Resource > Load |> Runtime
 *				   \ Resource > Save /              \ ...			  /
 */
struct ResourceManager final : Service
{
	/// @brief Pretty name of the service.
	constexpr static std::string_view service_name = RUKEN_STRING(ResourceManager);

	#pragma region Lifetime

	/// @brief Constructor
	explicit ResourceManager(ServiceProvider& in_provider) noexcept;
	ResourceManager(ResourceManager const&)	           = delete;
	ResourceManager(ResourceManager&&)		           = delete;
	ResourceManager& operator=(ResourceManager const&) = delete;
	ResourceManager& operator=(ResourceManager&&)	   = delete;
	~ResourceManager() override					       = default;

	#pragma endregion

	#pragma region Methods

	/// @brief Provides a new importer type to the library.
	template <typename TLoader> requires std::is_base_of_v<ResourceLoader, TLoader>
	RkVoid ProvideLoader() noexcept;

	/// @brief Tries to load the passed file.
	template <CResource TResource>
	ResourceHandle<TResource> Request(FilePath const& in_file_path);

	#pragma endregion

	private:

		#pragma region Members

		std::vector<std::unique_ptr<ResourceLoader>>   m_loaders         {};
		std::unordered_map<FilePath, ResourceManifest> m_manifests 		 {};
		std::mutex									   m_manifests_mutex {};

		#pragma endregion

		#pragma region Methods

		/// @brief Looks though the importers and returns the first compatible one or nullptr if not found.
		ResourceLoader* GetCompatibleLoader(std::filesystem::path const& in_extension) const;

		/// @brief Waits for in_loading_task to finish and exchanges the Resource pointer of the passed in_manifest.
		IOTask<RkVoid> Load(ResourceManifest* in_manifest, ResourceLoader const* in_loader) const noexcept;

		#pragma endregion
};


END_RUKEN_NAMESPACE

#include "Resources/ResourceManager.inl"