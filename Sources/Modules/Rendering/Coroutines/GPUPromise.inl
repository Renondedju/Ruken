#pragma once

BEGIN_RUKEN_NAMESPACE

template<typename ... TArgs>
GPUPromise::GPUPromise(RenderDevice& in_device, TArgs&&...) noexcept:
	device		   		{in_device},
	command_buffer 		(in_device.GetCommandBuffer()),
	end_execution_fence {in_device.GetDevice(), vk::FenceCreateInfo{
		.flags = {}
	}}
{}

END_RUKEN_NAMESPACE