#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief An enumeration of all the major locations or "volumes" of the filesystem.
enum class EFilesystemLocation
{
	ProjectDirectory,
	ImportedAssets,
	Temporary,
};

/// @brief Transforms a filesystem location to a string
inline std::string EFilesystemLocationToString(EFilesystemLocation const in_location)
{
	switch (in_location)
	{
		case EFilesystemLocation::ProjectDirectory:
			return "ProjectDirectory";
		case EFilesystemLocation::ImportedAssets:
			return "ImportedAssets";
		case EFilesystemLocation::Temporary:
			return "Temporary";

		default:
			std::unreachable();
	}
}

END_RUKEN_NAMESPACE