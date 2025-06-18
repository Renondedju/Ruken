#pragma once

#include "Filesystem/IOJobQueue.hpp"
#include "Filesystem/EOpenMode.hpp"
#include "Filesystem/FilePath.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Represents a file.
 * It could be from a regular filesystem, some kind of Cloud Solution or a compressed archive.
 */
struct File
{
	#pragma region Members

	const EOpenMode OpenMode;
	const FilePath  Path;

	#pragma endregion

	#pragma region Lifetime

	/**
	 * Default constructor
	 * @param in_path Path of the file.
	 * @param in_open_mode Open mode.
	 */
	explicit File(FilePath const& in_path, EOpenMode const in_open_mode) noexcept:
		OpenMode {in_open_mode},
		Path	 {in_path}
	{}

	File           (File const& in_copy) = default;
	File           (File&&      in_move) = default;
	File& operator=(File const& in_copy) = delete;
	File& operator=(File&&      in_move) = delete;
	virtual ~File() noexcept			 = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * Reads the whole file.
	 * @return File contents.
	 */
	virtual IOTask<std::vector<RkByte>> Read() = 0;

	#pragma endregion
};

using FileHandle = std::unique_ptr<File>;

END_RUKEN_NAMESPACE