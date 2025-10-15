#pragma once

#include "Core/Types/FundamentalTypes.hpp"

#include <vector>

BEGIN_RUKEN_NAMESPACE

struct ISerializable
{
	#pragma region Lifetime

	ISerializable()								   = default;
	ISerializable(ISerializable const&) 		   = default;
	ISerializable(ISerializable&&)				   = default;
	ISerializable& operator=(ISerializable const&) = default;
	ISerializable& operator=(ISerializable&&) 	   = default;
	virtual ~ISerializable()				  	   = default;

	#pragma endregion

	virtual std::vector<RkByte> Serialize() const   			   = 0;
	virtual RkVoid Deserialize(std::vector<RkByte> const& in_data) = 0;
};

END_RUKEN_NAMESPACE