#pragma once

#include "Rendering/RenderDevice.hpp"
#include "Rendering/Resources/GPUBuffer.hpp"
#include "Maths/Matrix/Matrix.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <ranges>
#include <vector>

BEGIN_RUKEN_NAMESPACE

using TransformBuffer = std::vector<Matrix4x4>;

struct TransformBufferStorage
{
	/**
	 * @param in_device Devices that stores camera data descriptor sets.
	 * @param in_pool Pool to allocate sets from.
	 * @param in_buffer_elements Size of the buffer.
	 * @param in_instances Amount of instances to allocate.
	 */
	explicit TransformBufferStorage(RenderDevice& in_device, vk::DescriptorPool const in_pool, RkSize const in_buffer_elements, RkUint32 const in_instances):
		buffer_elements {in_buffer_elements},
		buffer_size     {sizeof(Matrix4x4) * in_buffer_elements},
		binding {
			.binding		    = 0,
			.descriptorType     = in_instances == 1 ? vk::DescriptorType::eStorageBuffer : vk::DescriptorType::eStorageBuffer,
			.descriptorCount    = 1,
			.stageFlags		    = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
			.pImmutableSamplers = nullptr,
		},
		storage {in_device, vk::BufferCreateInfo {
			.flags                 = {},
			.size                  = buffer_size * in_instances,
			.usage                 = vk::BufferUsageFlagBits::eStorageBuffer,
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
		sets    {[&] {
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
			.range  = buffer_size,
		});

		for (auto const& [index, buffer_info] : std::ranges::views::enumerate(buffer_infos))
			buffer_info.offset = buffer_size * index;

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

	RkVoid Update(TransformBuffer const& in_data, RkUint32 const in_instance) const
	{
		RUKEN_ASSERT(sets   .size() >  in_instance,     "Cannot update an instance that does not exist");
		RUKEN_ASSERT(in_data.size() <= buffer_elements, "Data size should not be greater than the amount of elements contained in the buffer");

		storage.Update(buffer_size * in_instance, buffer_size, in_data.data());
	}

	RkSize								 buffer_elements;
	RkSize								 buffer_size;
	vk::DescriptorSetLayoutBinding		 binding;
	GPUBuffer				      	     storage;
	vk::raii::DescriptorSetLayout 	     layout;
	std::vector<vk::raii::DescriptorSet> sets;
};

END_RUKEN_NAMESPACE