#pragma once

#include "Filesystem/File.hpp"

BEGIN_RUKEN_NAMESPACE

using HANDLE = void*;

/**
 * File implementation of the underlying operating system's filesystem.
 */
struct WindowsFile final: File
{
	#pragma region Lifetime

	/**
	 * Default constructor
	 * @param in_path Path of the file.
	 * @param in_open_mode Open mode.
	 * @param in_os_path Full OS path.
	 */
	explicit WindowsFile(FilePath const& in_path, EOpenMode in_open_mode, std::filesystem::path const& in_os_path);
	WindowsFile           (WindowsFile const& in_copy) = default;
	WindowsFile           (WindowsFile&&      in_move) = default;
	WindowsFile& operator=(WindowsFile const& in_copy) = delete;
	WindowsFile& operator=(WindowsFile&&      in_move) = delete;
	~WindowsFile() noexcept override;

	#pragma endregion

	#pragma region Methods

	/**
	 * Asynchronously reads the whole file.
	 * @return Asynchronous task.
	 */
	IOTask<std::vector<RkByte>> Read() override;

	#pragma endregion

	private:

		#pragma region Members

		HANDLE		file_handle {nullptr};
		std::size_t file_size   {0};

		#pragma endregion
};

END_RUKEN_NAMESPACE
