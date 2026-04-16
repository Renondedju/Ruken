#pragma once

#include "Types/FundamentalTypes.hpp"

#include <vulkan/vulkan.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUImageAccess
{
	vk::Image				  image				{};
	vk::ImageLayout			  layout 		    {vk::ImageLayout		   ::eUndefined};
	vk::PipelineStageFlags2   stages 		    {vk::PipelineStageFlagBits2::eTopOfPipe};
	vk::AccessFlags2          access_flags      {};
	vk::ImageSubresourceRange subresource_range {};
	uint32_t				  family_index      {};

	RkBool operator==(GPUImageAccess const& in_other) const noexcept
	{
		return
			image              == in_other.image             &&
			layout             == in_other.layout            &&
			stages             == in_other.stages            &&
			access_flags       == in_other.access_flags      &&
			subresource_range  == in_other.subresource_range &&
			family_index       == in_other.family_index;
	}
};

END_RUKEN_NAMESPACE