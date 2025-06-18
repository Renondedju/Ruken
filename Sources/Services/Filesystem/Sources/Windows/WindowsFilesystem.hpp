#pragma once

#include "Filesystem/Filesystem.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Filesystem implementation of the underlying operating system's filesystem.
 */
struct WindowsFilesystem final: Filesystem
{
	#pragma region Members

	std::filesystem::path const project_path;
	std::filesystem::path const tmp_path;

	#pragma endregion

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_service_provider Service provider instance.
	 * @param in_project_path The root directory of the project, listed as ELocation::ProjectDirectory.
	 */
	explicit WindowsFilesystem(ServiceProvider& in_service_provider, std::filesystem::path const& in_project_path);
	WindowsFilesystem           (WindowsFilesystem const& in_copy) = delete;
	WindowsFilesystem           (WindowsFilesystem&&      in_move) = delete;
	WindowsFilesystem& operator=(WindowsFilesystem const& in_copy) = delete;
	WindowsFilesystem& operator=(WindowsFilesystem&&      in_move) = delete;
	~WindowsFilesystem()								  override = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * Opens a file.
	 * @param in_path Path of the file.
	 * @param in_open_mode Open mode.
	 * @return File instance.
	 */
	FileHandle Open(FilePath const& in_path, EOpenMode in_open_mode) override;

	/**
	 * Returns a path corresponding to the passed location.
	 * @param in_location Location.
	 * @return Path.
	 */
	std::filesystem::path const& GetPathFromLocation(EFilesystemLocation in_location) const noexcept;

	/**
	 * Returns the corresponding file mode string.
	 * @param in_open_mode Open mode.
	 * @return File mode string.
	 */
	static std::string_view const& GetFileModeString(EOpenMode in_open_mode) noexcept;

	#pragma endregion
};

END_RUKEN_NAMESPACE
