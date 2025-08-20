#pragma once

#include "Filesystem/FilePath.hpp"

BEGIN_RUKEN_NAMESPACE

struct ResourcePath
{
	FilePath			  file_path;
	std::filesystem::path subresource_name;
};

END_RUKEN_NAMESPACE

template<>
struct std::hash<RUKEN_NAMESPACE::ResourcePath>
{
	std::size_t operator()(RUKEN_NAMESPACE::ResourcePath const& in_path) const noexcept
	{
		std::size_t const h1 = std::hash<RUKEN_NAMESPACE::FilePath>{}(in_path.file_path);
		std::size_t const h2 = std::hash<std::filesystem::path    >{}(in_path.subresource_name);
		return h1 ^ (h2 << 1);
	}
};