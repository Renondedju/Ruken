#pragma once

#include "Filesystem/DirectoryPath.hpp"

#include <string>

BEGIN_RUKEN_NAMESPACE

/// @brief Path to a file in the current filesystem.
struct FilePath
{
	std::string   Filename;
	DirectoryPath Directory;

	std::string ToString() const noexcept
	{ return Directory.ToString() + "/" + Filename; }
};

END_RUKEN_NAMESPACE

template<>
struct std::hash<RUKEN_NAMESPACE::FilePath>
{
	std::size_t operator()(const RUKEN_NAMESPACE::FilePath& in_path) const noexcept
	{
		return std::hash<std::string>{}(in_path.ToString());
	}
};