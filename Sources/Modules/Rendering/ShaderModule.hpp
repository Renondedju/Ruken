#pragma once

#include "Resources/ResourceData.hpp"
#include "Resources/ResourcePath.hpp"
#include "Rendering/RenderDevice.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct ShaderModule final : ResourceData
{
	#pragma region Lifetime

	explicit ShaderModule(ServiceProvider     const& in_service_provider,
						  std::vector<RkByte> const& in_spirv_code) noexcept;

	ShaderModule(ShaderModule const&) 		     = delete;
	ShaderModule(ShaderModule&&)      		     = default;
	ShaderModule& operator=(ShaderModule const&) = delete;
	ShaderModule& operator=(ShaderModule&&)	     = default;
	~ShaderModule() override					 = default;

	#pragma endregion

	#pragma region Members

	vk::raii::ShaderModule						   module;
	std::vector<vk::PipelineShaderStageCreateInfo> stages;

	vk::raii::PipelineLayout layout;
	vk::raii::Pipeline		 pipeline;

	#pragma endregion
};

END_RUKEN_NAMESPACE
