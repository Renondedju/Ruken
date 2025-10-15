#pragma once

#include "Filesystem/Filesystem.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Filesystem implementation of the underlying operating system's filesystem.
 */
struct StdFilesystem final: Filesystem
{
	#pragma region Members

	std::filesystem::path const project_path;
	std::filesystem::path const imported_assets_path;
	std::filesystem::path const tmp_path;

	#pragma endregion

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_service_provider Service provider instance.
	 * @param in_project_path The root directory of the project, listed as ELocation::ProjectDirectory.
	 */
	explicit StdFilesystem(ServiceProvider& in_service_provider, std::filesystem::path const& in_project_path);
	StdFilesystem           (StdFilesystem const& in_copy) = delete;
	StdFilesystem           (StdFilesystem&&      in_move) = delete;
	StdFilesystem& operator=(StdFilesystem const& in_copy) = delete;
	StdFilesystem& operator=(StdFilesystem&&      in_move) = delete;
	~StdFilesystem()								  override = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * Opens a file.
	 * @param in_path Path of the file.
	 * @return File instance.
	 */
	FileHandle Open(FilePath const& in_path) override;

	/**
	 * Returns a path corresponding to the passed location.
	 * @param in_location Location.
	 * @return Path.
	 */
	std::filesystem::path GetPathFromLocation(EFilesystemLocation in_location) const noexcept;

	#pragma endregion
};

END_RUKEN_NAMESPACE
