#pragma once

#include "Filesystem/EFilesystemLocation.hpp"

#include <string>

BEGIN_RUKEN_NAMESPACE

/// @brief Path to a directory in the current filesystem.
struct DirectoryPath
{
	EFilesystemLocation Location;
	std::string         Path;
};

END_RUKEN_NAMESPACE