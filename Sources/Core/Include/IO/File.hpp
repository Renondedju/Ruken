#pragma once

#include "IO/IOJobQueue.hpp"
#include "JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp"
#include "Utility/WindowsOS.hpp"

#include <tracy/Tracy.hpp>

BEGIN_RUKEN_NAMESPACE

enum class EOpenMode
{
	Read  = 1 << 1,
	Write = 1 << 2,
};

struct File
{
	static inline AutomaticResetEvent on_io_pull {};

	#pragma region Lifetime

	explicit File(std::filesystem::path const& in_path);
	 File& operator=(File&&     ) = default;
	 File& operator=(File const&) = default;
	 File			(File&&     ) = default;
	 File			(File const&) = default;
	~File();

	#pragma endregion

	IOTask<std::vector<RkByte>> ReadEverything() const noexcept
	{
		LARGE_INTEGER       size           {};
		std::vector<RkByte> buffer         {};
		DWORD               bytes_read     {};
		OVERLAPPED          read_operation {};
		const HANDLE        file {CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr)};

		if (file == INVALID_HANDLE_VALUE)
			throw WindowsException();

		TracyMessageL("GetFileSizeEx");

		// Reading file size and allocating the buffer
		if (!GetFileSizeEx(file, &size))
			throw WindowsException();

		TracyMessageL("Alloc");

		buffer.resize(size.LowPart);

		// Issuing the read operation
		read_operation.Offset = 0;
		read_operation.hEvent = CreateEvent(nullptr, true, false, nullptr);
		if (!read_operation.hEvent)
			throw WindowsException();

		TracyMessageL("ReadFileEx");

		if (!::ReadFile(file, buffer.data(), size.LowPart, nullptr, &read_operation))
			if (const DWORD error {GetLastError()}; error != ERROR_IO_PENDING)
				throw WindowsException(error);

		// Waiting for the data to be read
		TracyMessageL("GetOverlappedResult");

		while(!GetOverlappedResult(file, &read_operation, &bytes_read, false))
		{
			if (const DWORD error {GetLastError()}; error != ERROR_IO_PENDING && error != ERROR_IO_INCOMPLETE)
				throw WindowsException(error);

			co_await on_io_pull;
		}

		TracyMessageL("CloseHandle");

		// Cleanup
		CloseHandle(read_operation.hEvent);
		CloseHandle(file);

		co_return buffer;
	}

	const std::filesystem::path path;
};

inline File::File(std::filesystem::path const& in_path):
	path(in_path)
{}

inline File::~File()
{}

END_RUKEN_NAMESPACE
