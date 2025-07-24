#pragma once

#include "Filesystem/DirectoryPath.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Path to a file in the current filesystem.
struct FilePath
{
	DirectoryPath Directory;
	std::string   Filename;
};

END_RUKEN_NAMESPACE