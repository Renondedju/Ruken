#pragma once

#include "Filesystem/EFilesystemLocation.hpp"

#include <filesystem>

BEGIN_RUKEN_NAMESPACE

/// @brief Path to a directory in the current filesystem.
struct FilePath
{
	EFilesystemLocation   location;
	std::filesystem::path path;

	std::string ToString() const noexcept
	{ return (EFilesystemLocationToString(location) / path).generic_string(); }
};

END_RUKEN_NAMESPACE

template<>
struct std::hash<RUKEN_NAMESPACE::FilePath>
{
	std::size_t operator()(RUKEN_NAMESPACE::FilePath const& in_path) const noexcept
	{
		using UnderlyingType = std::underlying_type_t<RUKEN_NAMESPACE::EFilesystemLocation>;

		std::size_t const h1 = std::hash<UnderlyingType		  >{}(static_cast<UnderlyingType>(in_path.location));
		std::size_t const h2 = std::hash<std::filesystem::path>{}(in_path.path);
		return h1 ^ (h2 << 1);
	}
};