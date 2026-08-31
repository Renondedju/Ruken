#pragma once

#include "Maths/Matrix/Matrix.hpp"
#include "Maths/Vector/DistanceVector.hpp"

#include "Rendering/RenderDevice.hpp"
#include "Rendering/Resources/GPUBuffer.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vector>
#include <ranges>

BEGIN_RUKEN_NAMESPACE

struct CameraData
{
	explicit CameraData(Matrix4x4 const& in_view, Matrix4x4 const& in_projection) noexcept:
		view					 {in_view},
		projection				 {in_projection},
		view_projection 		 {view * projection},
		inverted_view			 {view		.Inverted()},
		inverted_projection 	 {projection.Inverted()},
		inverted_view_projection {inverted_projection * inverted_view},
		world_position			 {Meters(inverted_view.data[12]), Meters(inverted_view.data[13]), Meters(inverted_view.data[14])},
		near_plane_position      {(inverted_view_projection * Vector4m(0_m, 0_m, -1_m, 1_m)).Homogenize()},
		far_plane_position       {(inverted_view_projection * Vector4m(0_m, 0_m,  1_m, 1_m)).Homogenize()},
		forward					 {(far_plane_position - near_plane_position).Normalized()}
	{}

	// This struct is uploaded to the GPU as is.
	// Modifications here should be matched with the slang counterpart.
	alignas(16) Matrix4x4 view;
	alignas(16) Matrix4x4 projection;
	alignas(16) Matrix4x4 view_projection;

	alignas(16) Matrix4x4 inverted_view;
	alignas(16) Matrix4x4 inverted_projection;
	alignas(16) Matrix4x4 inverted_view_projection;

	alignas(16) Vector3m  world_position;
	alignas(16) Vector3m  near_plane_position;
	alignas(16) Vector3m  far_plane_position;
	alignas(16) Vector3m  forward;
};

/**
 * In order for shader code to access data, vulkan requires us to create pointer or handles
 * that also contains properties about the said data (descriptors).
 * These handles are packed in groups (descriptor sets) and bound to some slot on the gpu before to make that data
 * available to shader code.
 *
 * This class handles allocation of the backing buffer and allocation of a set.
 * It is also possible to allocate space for multiple instances of the same data type to enable pipelining of GPU work (double/triple buffering).
 */
struct CameraDataStorage
{
	/**
	 * @param in_device Devices that stores camera data descriptor sets.
	 * @param in_pool Pool to allocate sets from.
	 * @param in_instances Amount of instances to allocate.
	 */
	explicit CameraDataStorage(RenderDevice& in_device, vk::DescriptorPool const in_pool, RkUint32 const in_instances):
		binding  {
			.binding		    = 0,
			.descriptorType     = vk::DescriptorType::eUniformBuffer,
			.descriptorCount    = 1,
			.stageFlags		    = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
			.pImmutableSamplers = nullptr,
		},
		storage {in_device, vk::BufferCreateInfo {
			.flags                 = {},
			.size                  = sizeof(CameraData) * in_instances,
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
			.range  = sizeof(CameraData),
		});

		for (auto const& [index, buffer_info] : std::ranges::views::enumerate(buffer_infos))
			buffer_info.offset = sizeof(CameraData) * index;

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

	RkVoid Update(CameraData const& in_data, RkUint32 const in_instance) const
	{
		RUKEN_ASSERT(sets.size() > in_instance, "Cannot update an instance that does not exist");

		storage.Update(sizeof(CameraData) * in_instance, sizeof(CameraData), &in_data);
	}

	vk::DescriptorSetLayoutBinding		 binding;
	GPUBuffer				      	     storage;
	vk::raii::DescriptorSetLayout 	     layout;
	std::vector<vk::raii::DescriptorSet> sets;
};

END_RUKEN_NAMESPACE
