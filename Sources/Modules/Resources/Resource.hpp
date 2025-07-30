#pragma once

#include "Build/Namespace.hpp"
#include "Filesystem/FilePath.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief The base resource class.
struct Resource
{
	#pragma region Lifetime

	Resource()							 = default;
	Resource(Resource const&)			 = default;
	Resource(Resource&&     )			 = default;
	Resource& operator=(Resource const&) = default;
	Resource& operator=(Resource&&     ) = default;
	virtual ~Resource()					 = default;

	#pragma endregion

	#pragma region Members

	FilePath    file_path		 {};
	std::string subresource_name {};

	#pragma endregion
};

END_RUKEN_NAMESPACE