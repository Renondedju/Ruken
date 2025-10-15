#pragma once

#include "Filesystem/FilePath.hpp"

BEGIN_RUKEN_NAMESPACE

struct ResourcePath
{
	FilePath asset_file    {};
	FilePath resource_file {};
};

END_RUKEN_NAMESPACE

template<>
struct std::hash<RUKEN_NAMESPACE::ResourcePath>
{
	std::size_t operator()(RUKEN_NAMESPACE::ResourcePath const& in_path) const noexcept
	{
		return std::hash<RUKEN_NAMESPACE::FilePath>{}(in_path.resource_file);
	}
};