#pragma once

#include "Filesystem/EFilePosition.hpp"
#include "Filesystem/File.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * File implementation of the underlying operating system's filesystem.
 */
struct StdFile final: File
{
	#pragma region Lifetime

	/**
	 * Default constructor
	 * @param in_path Path of the file.
	 * @param in_os_path Full OS path.
	 */
	explicit StdFile(FilePath const& in_path, std::filesystem::path const& in_os_path);
	StdFile           (StdFile const& in_copy) = default;
	StdFile           (StdFile&&      in_move) = default;
	StdFile& operator=(StdFile const& in_copy) = delete;
	StdFile& operator=(StdFile&&      in_move) = delete;
	~StdFile() noexcept override;

	#pragma endregion

	#pragma region Methods

	/**
	 * Reads a number of bytes starting from the file pointer.
	 * The file pointer also gets pushed by the number of bytes read.
	 *
	 * @param in_destination Address of a buffer to write into.
	 * @param in_start_position Start position of the read.
	 * @param in_size Number of bytes to read.
	 * @return Number of bytes actually read.
	 */
	IOTask<RkSize> Read(RkVoid* in_destination, FileCursor in_start_position, RkSize in_size) const override;

	/**
	 * Writes a number of bytes starting from the file pointer.
	 * The file pointer also gets pushed by the number of bytes written.
	 *
	 * @param in_source Address of a buffer to read from.
	 * @param in_start_position Start position.
	 * @param in_size Number of bytes to write.
	 * @return Number of bytes actually written.
	 */
	IOTask<RkSize> Write(RkVoid const* in_source, FileCursor in_start_position, RkSize in_size) override;

	/// @brief Returns the size in bytes of the file.
	RkSize GetFileSize() const override;

	#pragma endregion

	private:

		#pragma region Members

		FILE* file_handle {nullptr};

		#pragma endregion

		#pragma region Methods

		/**
		 * Converts a file position into a move method for the windows.h SetFilePointer function.
		 * @param in_position File position.
		 * @return Move method.
		 */
		static int GetOrigin(EFilePosition in_position) noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE
