#pragma once

#include "Resources/IResourceData.hpp"
#include "Resources/ResourcePath.hpp"
#include "Rendering/RenderDevice.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

// TODO: This is a workaround.
// The resource should be the ShaderModule but it would require serialization to store
// the result of shader reflexion alongside spirv code.
// Currently that breaks resource reloading.
struct SpirvCode final : IResourceData
{
	explicit SpirvCode(std::vector<RkByte>&& in_code) noexcept:
		spirv_code {in_code}
	{}

	SpirvCode(SpirvCode const&) 		   = default;
	SpirvCode(SpirvCode&&)      		   = default;
	SpirvCode& operator=(SpirvCode const&) = default;
	SpirvCode& operator=(SpirvCode&&)	   = default;
	~SpirvCode() override				   = default;

	std::vector<RkByte> spirv_code;
};

struct ShaderModule
{
	#pragma region Lifetime

	explicit ShaderModule(RenderDevice&								  in_device,
						  SpirvCode							   const& in_spirv_code,
						  std::vector<vk::DescriptorSetLayout> const& in_set_layouts) noexcept;

	ShaderModule(ShaderModule const&) 		     = delete;
	ShaderModule(ShaderModule&&)      		     = default;
	ShaderModule& operator=(ShaderModule const&) = delete;
	ShaderModule& operator=(ShaderModule&&)	     = default;
	~ShaderModule()         					 = default;

	#pragma endregion

	#pragma region Members

	vk::raii::ShaderModule						   module;
	std::vector<vk::PipelineShaderStageCreateInfo> stages;
	vk::raii::PipelineLayout			  pipeline_layout;
	vk::raii::Pipeline							 pipeline;

	#pragma endregion
};

END_RUKEN_NAMESPACE
