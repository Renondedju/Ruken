#include "Rendering/RenderDevice.hpp"
#include "Core/ServiceProvider.hpp"

#include <thread>
#include <ranges>
#include <vulkan/vulkan_raii.hpp>

USING_RUKEN_NAMESPACE

RenderDevice::RenderDevice(ServiceProvider& in_parent):
	Service			     {in_parent, typeid(RenderDevice)},
	m_instance           {in_parent.LocateService<VulkanInstance>()},
	m_physical_device    {SelectPhysicalDevice()},
	m_queue_priorities   {1.0f},
	m_queue_create_infos {MakeQueueCreateInfo()},
	m_device             {[&] {

		// TODO: Feature sets (containing device features, extensions
		//		 and ways to check for compatibility with a RenderDevice)
		// Enabling core features
		vk::StructureChain features { m_physical_device.getFeatures2<
			vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceVulkan11Features,
			vk::PhysicalDeviceVulkan13Features
		>()};

		return m_physical_device.createDevice(vk::DeviceCreateInfo {
			.pNext 					 = &features.get<>(),
			.queueCreateInfoCount    = static_cast<uint32_t>(m_queue_create_infos.size()),
			.pQueueCreateInfos 		 = m_queue_create_infos.data(),
			.enabledLayerCount 		 = 0u,	      // Deprecated and ignored.
			.ppEnabledLayerNames	 = nullptr,  // Deprecated and ignored.
			.enabledExtensionCount	 = static_cast<uint32_t>(s_extensions.size()),
			.ppEnabledExtensionNames = s_extensions.data(),
			.pEnabledFeatures		 = nullptr // Deprecated
		});
	}()},
	m_queues {m_device.getQueue2(vk::DeviceQueueInfo2 {
		.queueFamilyIndex = m_queue_create_infos[0].queueFamilyIndex,
		.queueIndex		  = 0u
	})},
	m_command_pool    {m_device.createCommandPool(vk::CommandPoolCreateInfo {
		.flags 			  = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = m_queue_create_infos[0].queueFamilyIndex
	})},
	m_command_buffers {m_device, vk::CommandBufferAllocateInfo {
		.commandPool 		= m_command_pool,
		.level		 		= vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = std::thread::hardware_concurrency()
	}}
{}

std::vector<vk::DeviceQueueCreateInfo> RenderDevice::MakeQueueCreateInfo() const noexcept
{
	RkUint32    graphics_index   {0u};
	std::vector queue_properties {m_physical_device.getQueueFamilyProperties()};

	// For now, we only look for a queue with graphics capabilities
	for (auto&& [index, property] : std::ranges::views::enumerate(queue_properties))
		if (property.queueFlags & vk::QueueFlagBits::eGraphics)
			{ graphics_index = index; break; }

	return std::vector {
		vk::DeviceQueueCreateInfo {
			.sType 			  = vk::StructureType::eDeviceQueueCreateInfo,
			.pNext 			  = nullptr,
			.flags 			  = {},
			.queueFamilyIndex = graphics_index,
			.queueCount		  = 1u,
			.pQueuePriorities = &m_queue_priorities[0],
		}
	};
}

vk::raii::PhysicalDevice RenderDevice::SelectPhysicalDevice() const noexcept
{
	// For now, we will only select the first device
	return m_instance->instance.enumeratePhysicalDevices()[0];
}
