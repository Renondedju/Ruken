#pragma once

BEGIN_RUKEN_NAMESPACE

template<typename ... TArgs>
GPUPromise::GPUPromise(RenderDevice& in_device, TArgs&&... in_args) noexcept:
	command_buffer (in_device.GetCommandBuffer())
{}

END_RUKEN_NAMESPACE