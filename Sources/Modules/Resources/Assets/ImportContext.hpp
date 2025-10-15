#pragma once

#include "Core/ServiceProvider.hpp"
#include "Filesystem/File.hpp"

BEGIN_RUKEN_NAMESPACE

struct ImportContext
{
	struct ResourceData
	{
		std::vector<RkByte> data {};
		FilePath			path {};
	};

	ServiceProvider const&	  services;
	FileHandle const		  asset_file {};
	std::vector<ResourceData> resources  {};
};

END_RUKEN_NAMESPACE