#include "Filesystem/Windows/WindowsFilesystem.hpp"
#include "Filesystem/Windows/WindowsFile.hpp"
#include "Debug/Logging/Logger.hpp"
 err
USING_RUKEN_NAMESPACE

WindowsFilesystem::WindowsFilesystem(ServiceProvider& in_service_provider, std::filesystem::path const& in_project_path):
	Filesystem   {in_service_provider},
	project_path {in_project_path},
	tmp_path     {std::filesystem::temp_directory_path()}
{
	if (auto const logger {m_service_provider.LocateService<Logger>()})
		logger->Log(service_name, ELogLevel::Info, "Opened a Windows filesystem at {}",
			std::filesystem::absolute(in_project_path).string()
		);
}

FileHandle WindowsFilesystem::Open(FilePath const& in_path)
{
	std::filesystem::path real_path {GetPathFromLocation(in_path.Directory.Location).c_str()};
	real_path += "\\";
	real_path += in_path.Directory.Path;
	real_path += "\\";
	real_path += in_path.Filename;

	if (auto const logger {m_service_provider.LocateService<Logger>()})
		logger->Log(service_name, ELogLevel::Info, "Opening file named {}", real_path.string());

	return std::make_unique<WindowsFile>(in_path, real_path);
}

std::filesystem::path WindowsFilesystem::GetPathFromLocation(EFilesystemLocation const in_location) const noexcept
{
	switch (in_location)
	{
		case EFilesystemLocation::ProjectDirectory:
			return std::filesystem::absolute(project_path);
		case EFilesystemLocation::Temporary:
			return std::filesystem::absolute(tmp_path);

		default:
			std::unreachable();
	}
}