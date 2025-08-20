#pragma once

#include "Resources/ResourcePath.hpp"
#include "Filesystem/ISerializable.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief The base resource class.
struct Resource : ISerializable
{
	#pragma region Lifetime

	explicit Resource(ResourcePath const& in_resource_path) noexcept:
		resource_path {in_resource_path}
	{}

	Resource(Resource const&)			 = default;
	Resource(Resource&&     )			 = default;
	Resource& operator=(Resource const&) = default;
	Resource& operator=(Resource&&     ) = default;
	~Resource() override				 = default;

	#pragma endregion

	#pragma region Members

	ResourcePath resource_path;

	#pragma endregion
};

END_RUKEN_NAMESPACE
