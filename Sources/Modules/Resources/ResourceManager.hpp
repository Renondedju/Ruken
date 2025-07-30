#pragma once

#include "Core/Meta/Meta.hpp"
#include "Core/Service.hpp"

#include "Resources/Resource.hpp"

#include "Filesystem/FilePath.hpp"
#include "Filesystem/IOJobQueue.hpp"

#include <unordered_map>

BEGIN_RUKEN_NAMESPACE

/**
 * Imports and manages asset's lifetime.
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

	#pragma region Methods

	/**
	 * Imports a file.
	 * @param in_file_path Path to the file to import.
	 */
	IOTask<RkVoid> Import(FilePath const& in_file_path);

	#pragma endregion

	private:

		#pragma region Members

		//std::unordered_multimap<std::string, Importer*> importers {};
		std::unordered_map	   <FilePath   , Resource*> resources {};

		#pragma endregion
};

END_RUKEN_NAMESPACE