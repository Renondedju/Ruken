#pragma once

#include "Core/ServiceProvider.hpp"

#include "Filesystem/File.hpp"
#include "Resources/Resource.hpp"

BEGIN_RUKEN_NAMESPACE

struct ImportContext
{
	struct ResourceData
	{
		ResourcePath		name {};
		std::vector<RkByte> data {};
	};

	ServiceProvider const&	  services;
	FileHandle				  asset_file {};
	std::vector<ResourceData> resources  {};
};

END_RUKEN_NAMESPACE