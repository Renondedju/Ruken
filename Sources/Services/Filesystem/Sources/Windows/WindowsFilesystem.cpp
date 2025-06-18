#include "Filesystem/Windows/WindowsFilesystem.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Filesystem/Windows/WindowsFile.hpp"

#include "Meta/Assert.hpp"

USING_RUKEN_NAMESPACE

WindowsFilesystem::WindowsFilesystem(ServiceProvider& in_service_provider, std::filesystem::path const& in_project_path):
	Filesystem   {in_service_provider},
	project_path {in_project_path},
	tmp_path     {std::filesystem::temp_directory_path()}
{
	RUKEN_ASSERT(in_project_path.is_absolute() && in_project_path.is_directory(),
		"Project path must be an absolute directory"
	);
}

FileHandle WindowsFilesystem::Open(FilePath const& in_path, EOpenMode const in_open_mode)
{
	std::filesystem::path real_path {GetPathFromLocation(in_path.Directory.Location).c_str()};
	real_path += "/";
	real_path += in_path.Directory.Path;
	real_path += "/";
	real_path += in_path.Filename;

	if (auto const logger {m_service_provider.LocateService<Logger>()})
		logger->Log(service_name, ELogLevel::Info, "Opened file named {}", real_path.string());

	return std::make_unique<WindowsFile>(in_path, in_open_mode, real_path);
}

std::filesystem::path const& WindowsFilesystem::GetPathFromLocation(EFilesystemLocation const in_location) const noexcept
{
	switch (in_location)
	{
		case EFilesystemLocation::ProjectDirectory:
			return project_path;
		case EFilesystemLocation::Temporary:
			return tmp_path;

		default:
			std::unreachable();
	}
}

std::string_view const& WindowsFilesystem::GetFileModeString(EOpenMode const in_open_mode) noexcept
{
	static std::string_view read_str  {"r"};
	static std::string_view write_str {"w+"};

	switch (in_open_mode)
	{
		case EOpenMode::Read:
			return read_str;
		case EOpenMode::Write:
			return write_str;

		default:
			std::unreachable();
	}
}
