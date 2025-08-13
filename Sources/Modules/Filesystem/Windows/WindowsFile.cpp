#include "Filesystem/EFilePosition.hpp"
#include "Filesystem/Windows/WindowsFile.hpp"
#include "Filesystem/Windows/WindowsFilesystem.hpp"

#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

WindowsFile::WindowsFile(FilesystemPath const& in_path, std::filesystem::path const& in_os_path):
	File {in_path}
{
	auto file_path = in_os_path.generic_string();
	file_handle    = std::fopen(file_path.c_str(), "a+");

	if (!file_handle)
		throw Exception(std::format("Failed to open file named : {}", file_path));
}

WindowsFile::~WindowsFile() noexcept
{
	std::fclose(file_handle);
}

IOTask<RkSize> WindowsFile::Read(RkVoid* in_destination, FileCursor const in_start_position, RkSize const in_size) const
{
	std::fseek(file_handle, in_start_position.offset, GetOrigin(in_start_position.position));
	std::size_t bytes_read {std::fread(in_destination, sizeof(RkByte), in_size, file_handle)};

	// TODO: Check if ferror and errno could catch an error from another thread.
	if (std::ferror(file_handle))
		throw ErrnoException(errno);

	co_return bytes_read;
}

IOTask<RkSize> WindowsFile::Write(RkVoid const* in_source, FileCursor const in_start_position, RkSize const in_size)
{
	std::fseek(file_handle, in_start_position.offset, GetOrigin(in_start_position.position));
	std::size_t bytes_written {std::fwrite(in_source, sizeof(RkByte), in_size, file_handle)};

	// TODO: Check if ferror and errno could catch an error from another thread.
	if (std::ferror(file_handle))
		throw ErrnoException(errno);

	co_return bytes_written;
}

RkSize WindowsFile::GetFileSize() const
{
	std::fseek(file_handle, 0, SEEK_END);
	return std::ftell(file_handle);
}

int WindowsFile::GetOrigin(EFilePosition const in_position) noexcept
{
	switch (in_position)
	{
		case EFilePosition::End:
			return SEEK_END;
		case EFilePosition::Current:
			return SEEK_CUR;
		case EFilePosition::Beginning:
			return SEEK_SET;

		default:
			std::unreachable();
	}
}
