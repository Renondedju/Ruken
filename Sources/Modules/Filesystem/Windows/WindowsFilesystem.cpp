#include "Filesystem/Windows/WindowsFilesystem.hpp"
#include "Filesystem/Windows/WindowsFile.hpp"
#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

WindowsFilesystem::WindowsFilesystem(ServiceProvider& in_service_provider, std::filesystem::path const& in_project_path):
	Filesystem           {in_service_provider},
	project_path         {in_project_path},
    imported_assets_path {in_project_path / "Imports"},
	tmp_path			 {std::filesystem::temp_directory_path()}
{
	if (auto const logger {m_service_provider.LocateService<Logger>()})
		logger->Log(service_name, ELogLevel::Info, "Opened a Windows filesystem at {}",
			std::filesystem::absolute(in_project_path).string()
		);
}

FileHandle WindowsFilesystem::Open(FilePath const& in_path)
{
	std::filesystem::path real_path {(
		GetPathFromLocation(in_path.Directory.Location) / in_path.Directory.Path / in_path.Filename
	).c_str()};

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
		case EFilesystemLocation::ImportedAssets:
			return std::filesystem::absolute(imported_assets_path);

		default:
			std::unreachable();
	}
}