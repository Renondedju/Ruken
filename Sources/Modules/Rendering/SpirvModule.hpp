#pragma once

#include "Resources/Resource.hpp"

BEGIN_RUKEN_NAMESPACE

struct SpirvModule final : Resource
{
	#pragma region Lifetime

	SpirvModule(FilesystemPath 	    const& in_file_path,
				std::string    	    const& in_subresource_name,
				std::vector<RkByte> const& in_data) noexcept:
		Resource {in_file_path, in_subresource_name},
		data     {in_data}
	{}

	SpirvModule(SpirvModule const&) 		   = default;
	SpirvModule(SpirvModule&&)      		   = default;
	SpirvModule& operator=(SpirvModule const&) = default;
	SpirvModule& operator=(SpirvModule&&)	   = default;
	~SpirvModule() override					   = default;

	#pragma endregion

	#pragma region Methods

	/// @copydoc ISerializable::Serialize
	std::vector<RkByte> Serialize() const override
	{ return data; }

	/// @copydoc ISerializable::Deserialize
	RkVoid Deserialize(std::vector<RkByte> const& in_data) override
	{ data = in_data; }

	#pragma endregion

	#pragma region Members

	std::vector<RkByte> data;

	#pragma endregion
};

END_RUKEN_NAMESPACE