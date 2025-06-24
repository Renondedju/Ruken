#include "Filesystem/EFilePosition.hpp"
#include "Filesystem/Windows/WindowsFile.hpp"
#include "Filesystem/Windows/WindowsFilesystem.hpp"

#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

WindowsFile::WindowsFile(FilePath const& in_path, std::filesystem::path const& in_os_path):
	File {in_path}
{
	file_handle = CreateFileW(in_os_path.c_str(),
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr
	);

	if (file_handle == INVALID_HANDLE_VALUE)
		throw WindowsException();
}

WindowsFile::~WindowsFile() noexcept
{
	CloseHandle(file_handle);
}

IOTask<RkSize> WindowsFile::Read(RkVoid* in_destination, FileCursor const in_start_position, RkSize const in_size) const
{
	DWORD bytes_read {};

	SetFilePointer(file_handle, in_start_position.offset, nullptr, GetWindowsMoveMethod(in_start_position.position));
	if (ReadFile(file_handle, in_destination, in_size, &bytes_read, nullptr) == FALSE)
		throw WindowsException();

	co_return bytes_read;
}

IOTask<RkSize> WindowsFile::Write(RkVoid* in_source, FileCursor const in_start_position, RkSize const in_size)
{
	DWORD bytes_written {};

	SetFilePointer(file_handle, in_start_position.offset, nullptr, GetWindowsMoveMethod(in_start_position.position));
	if (WriteFile(file_handle, in_source, in_size, &bytes_written, nullptr) == FALSE)
		throw WindowsException();

	co_return bytes_written;
}

RkSize WindowsFile::GetFileSize() const
{
	return SetFilePointer(file_handle, 0, nullptr, FILE_END);
}

DWORD WindowsFile::GetWindowsMoveMethod(EFilePosition const in_position) noexcept
{
	switch (in_position)
	{
		case EFilePosition::End:
			return FILE_END;
		case EFilePosition::Beginning:
			return FILE_BEGIN;

		default:
			std::unreachable();
	}
}
