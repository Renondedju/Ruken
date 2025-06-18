#include "Filesystem/Windows/WindowsFile.hpp"
#include "Filesystem/Windows/WindowsFilesystem.hpp"

#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

WindowsFile::WindowsFile(FilePath const& in_path, EOpenMode const in_open_mode, std::filesystem::path const& in_os_path):
	File {in_path, in_open_mode}
{
	if (in_open_mode == EOpenMode::Read)
		file_handle = CreateFileW(in_os_path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	else
		file_handle = CreateFileW(in_os_path.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (file_handle == INVALID_HANDLE_VALUE)
		throw WindowsException();

	file_size = std::filesystem::file_size(in_os_path);
}

WindowsFile::~WindowsFile() noexcept
{
	if (file_handle != INVALID_HANDLE_VALUE)
		CloseHandle(file_handle);
}

IOTask<std::vector<RkByte>> WindowsFile::Read()
{
	std::vector<RkByte> buffer {};
	buffer.resize(file_size);

	DWORD bytes_read {};
	SetFilePointer(file_handle, 0, nullptr, FILE_BEGIN);
	if (ReadFile(file_handle, buffer.data(), file_size, &bytes_read, nullptr) == FALSE)
		throw WindowsException();

	co_return buffer;
}
