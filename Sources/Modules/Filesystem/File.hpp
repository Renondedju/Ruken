#pragma once

#include "Filesystem/FileCursor.hpp"
#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/FilePath.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Represents a file.
 * It could be from a regular filesystem, some kind of Cloud Solution or a compressed archive.
 */
struct File
{
	#pragma region Members

	const FilePath Path;

	#pragma endregion

	#pragma region Lifetime

	/**
	 * Default constructor
	 * @param in_path Path of the file.
	 */
	explicit File(FilePath const& in_path) noexcept:
		Path {in_path}
	{}

	File           (File const& in_copy) = default;
	File           (File&&      in_move) = default;
	File& operator=(File const& in_copy) = delete;
	File& operator=(File&&      in_move) = delete;
	virtual ~File() noexcept			 = default;

	#pragma endregion

	#pragma region Methods

	/// @brief Returns the size in bytes of the file.
	virtual RkSize GetFileSize() const = 0;

	/**
	 * Reads a number of bytes starting from the file pointer.
	 * The file pointer also gets pushed by the number of bytes read.
	 *
	 * @param in_destination Address of a buffer to write into.
	 * @param in_start_position Start position of the read.
	 * @param in_size Number of bytes to read.
	 * @return Number of bytes actually read.
	 */
	virtual IOTask<RkSize> Read(RkVoid* in_destination, FileCursor in_start_position, RkSize in_size) const = 0;

	/**
	 * Writes a number of bytes starting from the file pointer.
	 * The file pointer also gets pushed by the number of bytes written.
	 *
	 * @param in_source Address of a buffer to read from.
	 * @param in_start_position Start position of the write.
	 * @param in_size Number of bytes to write.
	 * @return Number of bytes actually written.
	 */
	virtual IOTask<RkSize> Write(RkVoid* in_source, FileCursor in_start_position, RkSize in_size) = 0;

	#pragma endregion
};

// File is meant to be a polymorphic type.
using FileHandle = std::shared_ptr<File>;

END_RUKEN_NAMESPACE