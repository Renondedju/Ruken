#pragma once

#include "RenderDevice.hpp"
#include "Maths/Vector/DistanceVector.hpp"

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

struct GridParameters
{
	// plane_normal is expected to be one of the origin vectors
	Vector3m plane_normal = Constants<Vector3m>::up;
	float    grid_opacity = 1.0f;
	float    axis_opacity = 1.0f;

	// Colors picked from blender's theme
	Vector3m x_axis_color = Vector3m(0.70_m, 0.32_m, 0.32_m);
	Vector3m y_axis_color = Vector3m(0.38_m, 0.53_m, 0.16_m);
	Vector3m z_axis_color = Vector3m(0.29_m, 0.50_m, 0.79_m);
};

struct GridParametersStorage
{
	/**
	 * @param in_device Devices that stores camera data descriptor sets.
	 * @param in_pool Pool to allocate sets from.
	 * @param in_instances Amount of instances to allocate.
	 */
	explicit GridParametersStorage(RenderDevice& in_device, vk::DescriptorPool const in_pool, RkUint32 const in_instances):
		binding  {
			.binding		    = 0,
			.descriptorType     = in_instances == 1 ? vk::DescriptorType::eUniformBuffer : vk::DescriptorType::eUniformBuffer,
			.descriptorCount    = 1,
			.stageFlags		    = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
			.pImmutableSamplers = nullptr,
		},
		storage {in_device, vk::BufferCreateInfo {
			.flags                 = {},
			.size                  = sizeof(GridParameters) * in_instances,
			.usage                 = vk::BufferUsageFlagBits::eUniformBuffer,
			.sharingMode           = vk::SharingMode::eExclusive,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices   = nullptr
		}, VmaAllocationCreateInfo {
			.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			.usage = VMA_MEMORY_USAGE_AUTO
		}},
		layout  {in_device.GetDevice(), vk::DescriptorSetLayoutCreateInfo {
			.flags		  = {},
			.bindingCount = 1,
			.pBindings    = &binding
		}},
		sets  {[&] {
			std::vector const layouts(in_instances, *layout);
			return in_device.GetDevice().allocateDescriptorSets(vk::DescriptorSetAllocateInfo {
				.descriptorPool		= in_pool,
				.descriptorSetCount = in_instances,
				.pSetLayouts		= layouts.data()
			});
		}()}
	{
		std::vector buffer_infos(in_instances, vk::DescriptorBufferInfo {
			.buffer = storage.buffer,
			.offset = 0,
			.range  = sizeof(GridParameters),
		});

		for (auto const& [index, buffer_info] : std::ranges::views::enumerate(buffer_infos))
			buffer_info.offset = sizeof(GridParameters) * index;

		in_device.GetDevice().updateDescriptorSets(
			/* Writes : */ sets | std::views::transform([&](vk::raii::DescriptorSet const& in_set) {
				return vk::WriteDescriptorSet {
					.dstSet           = in_set,
					.dstBinding       = binding.binding,
					.dstArrayElement  = 0,
					.descriptorCount  = binding.descriptorCount,
					.descriptorType   = binding.descriptorType,
					.pBufferInfo      = buffer_infos.data()
				};
			}) | std::ranges::to<std::vector>(),

			/* Copy : */ {}
		);
	}

	RkVoid Update(GridParameters const& in_data, RkUint32 const in_instance) const
	{
		RUKEN_ASSERT(sets.size() > in_instance, "Cannot update an instance that does not exist");

		storage.Update(sizeof(GridParameters) * in_instance, sizeof(GridParameters), &in_data);
	}

	vk::DescriptorSetLayoutBinding		 binding;
	GPUBuffer				      	     storage;
	vk::raii::DescriptorSetLayout 	     layout;
	std::vector<vk::raii::DescriptorSet> sets;
};

END_RUKEN_NAMESPACE