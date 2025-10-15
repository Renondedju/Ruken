#include "Rendering/RenderDevice.hpp"
#include "Core/ServiceProvider.hpp"

#include <thread>
#include <ranges>
#include <volk.h>
#include <vulkan/vulkan_raii.hpp>
#include <tracy/TracyVulkan.hpp>

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

RenderDevice::RenderDevice(ServiceProvider& in_parent):
	Service			     {in_parent, typeid(RenderDevice)},
	m_instance           {in_parent.LocateService<VulkanInstance>()},
	m_physical_device    {SelectPhysicalDevice()},
	m_name				 {m_physical_device.getProperties2().properties.deviceName.data()},
	m_queue_priorities   {1.0f},
	m_queue_create_infos {MakeQueueCreateInfo()},
	m_device             {[&] {

		// TODO: Feature sets (containing device features, extensions
		//		 and ways to check for compatibility with a RenderDevice)
		vk::StructureChain features { m_physical_device.getFeatures2<
			// Enabling core features
			vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceVulkan11Features,
			vk::PhysicalDeviceVulkan13Features,
			// Debug
			vk::PhysicalDeviceHostQueryResetFeatures
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
{
#ifdef RUKEN_TRACE_BUILD

	RkBool device_calibrated {false};
	RkBool host_calibrated   {false};
	for (auto const& time_domain : m_physical_device.getCalibrateableTimeDomainsEXT())
	{
		if (time_domain == vk::TimeDomainKHR::eDevice)
			device_calibrated = true;

		if constexpr (BuildInfo::SystemID == BuildInfo::EOperatingSystem::Windows)
			if (time_domain == vk::TimeDomainKHR::eQueryPerformanceCounter)
				host_calibrated = true;

		if constexpr (BuildInfo::SystemID == BuildInfo::EOperatingSystem::Linux)
			if (time_domain == vk::TimeDomainKHR::eClockMonotonic)
				host_calibrated = true;
	}

	if (device_calibrated && host_calibrated)
	{
		m_tracy_context = TracyVkContextHostCalibrated(*m_instance->instance, *m_physical_device, *m_device,
			m_instance->instance.getDispatcher()->vkGetInstanceProcAddr,
			m_device			.getDispatcher()->vkGetDeviceProcAddr
		)

		TracyVkContextName(m_tracy_context, m_name.data(), m_name.size())
	}
	else if (Logger const* logger {in_parent.LocateService<Logger>()})
		logger->Warning(service_name, "GPU tracing is unavailable for device named {}", m_name.data());

#endif
}

RenderDevice::~RenderDevice()
{
	TracyVkDestroy(m_tracy_context);
}

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
