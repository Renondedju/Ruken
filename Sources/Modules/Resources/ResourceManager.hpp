#pragma once

#include "Core/Meta/Meta.hpp"
#include "Core/Service.hpp"

#include "Resources/Resource.hpp"
#include "Resources/Assets/AssetImporter.hpp"

#include "Filesystem/FilesystemPath.hpp"
#include "Filesystem/IOJobQueue.hpp"

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

	/**
	 * Default constructor.
	 * @param in_parent Owning provider.
	 */
	explicit ResourceManager(ServiceProvider& in_parent);

	ResourceManager			  (const ResourceManager&) = delete;
	ResourceManager			  (ResourceManager&&)      = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&)      = delete;
	~ResourceManager() override						   = default;

	#pragma endregion

	private:

		#pragma region Members

		std::vector<std::unique_ptr<AssetImporter>> importers {};
		std::unordered_map<FilesystemPath, Resource*>  resources {};

		#pragma endregion

		#pragma region Methods



		#pragma endregion
};

END_RUKEN_NAMESPACE