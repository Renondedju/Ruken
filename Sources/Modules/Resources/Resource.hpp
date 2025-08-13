#pragma once

#include "Filesystem/FilesystemPath.hpp"
#include "Filesystem/ISerializable.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief The base resource class.
struct Resource : ISerializable
{
	#pragma region Lifetime

	Resource(FilesystemPath const& in_file_path, std::string const& in_subresource_name) noexcept:
		file_path		 {in_file_path},
		subresource_name {in_subresource_name}
	{}

	Resource(Resource const&)			 = default;
	Resource(Resource&&     )			 = default;
	Resource& operator=(Resource const&) = default;
	Resource& operator=(Resource&&     ) = default;
	~Resource() override				 = default;

	#pragma endregion

	#pragma region Members

	FilesystemPath file_path		{};
	std::string    subresource_name {};

	#pragma endregion
};

END_RUKEN_NAMESPACE
