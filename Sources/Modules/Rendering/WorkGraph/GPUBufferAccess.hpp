#pragma once

#include "Types/FundamentalTypes.hpp"

#include <vulkan/vulkan.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUBufferAccess
{
	vk::Buffer				buffer		 	   {};
	vk::PipelineStageFlags2 stages 		 	   {vk::PipelineStageFlagBits2::eTopOfPipe};
	vk::AccessFlags2        access_flags 	   {};
	vk::DeviceSize			offset		 	   {};
	vk::DeviceSize			size         	   {};
	uint32_t				family_index {};

	RkBool operator==(GPUBufferAccess const& in_other) const noexcept
	{
		return
			buffer             == in_other.buffer       &&
			stages             == in_other.stages       &&
			access_flags       == in_other.access_flags &&
			offset		       == in_other.offset		&&
			size		       == in_other.size         &&
			family_index == in_other.family_index;
	}
};

END_RUKEN_NAMESPACE