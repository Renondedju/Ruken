#include "Filesystem/EFilePosition.hpp"
#include "Filesystem/STD/StdFile.hpp"
#include "Filesystem/STD/StdFilesystem.hpp"

#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

StdFile::StdFile(FilePath const& in_path, std::filesystem::path const& in_os_path):
	File {in_path}
{
	auto file_path = in_os_path.generic_string();
	file_handle    = std::fopen(file_path.c_str(), "ab+");

	if (!file_handle)
		throw Exception(std::format("Failed to open file named : {}", file_path));
}

StdFile::~StdFile() noexcept
{
	std::fclose(file_handle);
}

IOTask<RkSize> StdFile::Read(RkVoid* in_destination, FileCursor const in_start_position, RkSize const in_size) const
{
	std::fseek(file_handle, in_start_position.offset, GetOrigin(in_start_position.position));
	std::size_t bytes_read {std::fread(in_destination, sizeof(RkByte), in_size, file_handle)};

	// TODO: Check if ferror and errno could catch an error from another thread.
	if (std::ferror(file_handle))
		throw ErrnoException(errno);

	co_return bytes_read;
}

IOTask<RkSize> StdFile::Write(RkVoid const* in_source, FileCursor const in_start_position, RkSize const in_size)
{
	std::fseek(file_handle, in_start_position.offset, GetOrigin(in_start_position.position));
	std::size_t bytes_written {std::fwrite(in_source, sizeof(RkByte), in_size, file_handle)};

	// TODO: Check if ferror and errno could catch an error from another thread.
	if (std::ferror(file_handle))
		throw ErrnoException(errno);

	co_return bytes_written;
}

RkSize StdFile::GetFileSize() const
{
	std::fseek(file_handle, 0, SEEK_END);
	return std::ftell(file_handle);
}

int StdFile::GetOrigin(EFilePosition const in_position) noexcept
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
