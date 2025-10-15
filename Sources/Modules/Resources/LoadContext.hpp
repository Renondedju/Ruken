#pragma once

#include "Filesystem/FilePath.hpp"
#include "Core/ServiceProvider.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct LoadContext
{
	ServiceProvider const& services;
	FilePath	    const  file_path {};
	std::vector<RkByte>    data		 {};
};

END_RUKEN_NAMESPACE