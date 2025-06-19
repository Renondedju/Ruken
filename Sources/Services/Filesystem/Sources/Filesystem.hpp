#pragma once

#include "Core/Service.hpp"
#include "Meta/Meta.hpp"

#include "Filesystem/EOpenMode.hpp"
#include "Filesystem/FilePath.hpp"
#include "Filesystem/File.hpp"

#include <filesystem>

BEGIN_RUKEN_NAMESPACE

/**
 * Represents a file system.
 * It could be a regular filesystem, some kind of Cloud Solution or a compressed archive.
 */
struct Filesystem : Service
{
	// Static name of the service, used by the kernel to report service errors
	constexpr static std::string_view service_name = RUKEN_STRING(Filesystem);

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_service_provider Service provider instance.
	 */
	explicit Filesystem(ServiceProvider& in_service_provider) noexcept;
	Filesystem           (Filesystem const& in_copy) = delete;
	Filesystem           (Filesystem&&      in_move) = delete;
	Filesystem& operator=(Filesystem const& in_copy) = delete;
	Filesystem& operator=(Filesystem&&      in_move) = delete;
	~Filesystem()				   noexcept override = default;

	#pragma endregion

	/**
	 * Opens a file.
	 * @param in_path Path of the file.
	 * @param in_open_mode Open mode.
	 * @return File instance.
	 */
	virtual FileHandle Open(FilePath const& in_path, EOpenMode in_open_mode) = 0;
};

END_RUKEN_NAMESPACE
