#include "Rendering/RenderDevice.hpp"
#include "Core/ServiceProvider.hpp"

#include <thread>
#include <ranges>
#include <volk.h>
#include <vulkan/vulkan_raii.hpp>
#include <tracy/TracyVulkan.hpp>

#include "Coroutines/GPUPromise.hpp"
#include "Debug/Logging/Logger.hpp"
#include "JobSystem/SyncWait.hpp"
#include "JobSystem/Awaitables/Primitives/SharedMutex.hpp"

USING_RUKEN_NAMESPACE

RenderDevice::RenderDevice(ServiceProvider& in_parent):
	Service			     {in_parent, typeid(RenderDevice)},
	m_instance           {in_parent.LocateService<VulkanInstance>()},
	m_physical_device    {SelectPhysicalDevice()},
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

		auto const queue_create_info {MakeQueueCreateInfo()};

		return m_physical_device.createDevice(vk::DeviceCreateInfo {
			.pNext 					 = &features.get<>(),
			.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_info.size()),
			.pQueueCreateInfos 		 = queue_create_info.data(),
			.enabledLayerCount 		 = 0u,	       // Deprecated and ignored.
			.ppEnabledLayerNames	 = nullptr,   // Deprecated and ignored.
			.enabledExtensionCount	 = static_cast<uint32_t>(s_extensions.size()),
			.ppEnabledExtensionNames = s_extensions.data(),
			.pEnabledFeatures		 = nullptr // Deprecated
		});
	}()},
	m_name		   {m_physical_device.getProperties2().properties.deviceName.data()},
	m_family_views {m_physical_device.getQueueFamilyProperties().size()}
{
	FetchQueues();

#ifdef RUKEN_TRACE_BUILD
	InitTracyVkContext();
#endif
}

RenderDevice::~RenderDevice()
{
	TracyVkDestroy(m_tracy_context);
}

vk::raii::Instance& RenderDevice::GetInstance() const noexcept
{
	return m_instance->instance;
}

vk::raii::PhysicalDevice& RenderDevice::GetPhysicalDevice() noexcept
{
	return m_physical_device;
}

vk::raii::Device& RenderDevice::GetDevice() noexcept
{
	return m_device;
}

TracyVkCtx RenderDevice::TracyContext() const noexcept
{
	return m_tracy_context;
}

SharedMutex<FamilyView>* RenderDevice::FindQueueFamily(vk::QueueFlags const in_queue_flags)
{
	std::vector queue_properties {m_physical_device.getQueueFamilyProperties()};
	std::pair	best			 {
		std::numeric_limits<RkUint32>::max(), // index
		std::numeric_limits<RkUint32>::max() // score
	};

	// --- 1. Selecting & Ranking families
	for (auto&& [index, property] : std::views::enumerate(queue_properties))
	{
		// Ignoring queues that doesn't match the target type
		if ((property.queueFlags & in_queue_flags) != in_queue_flags)
			continue;

		// Score evaluation
		if (RkUint32 const score = std::popcount(static_cast<VkFlags>(property.queueFlags)); score < best.second)
			best = {index, score};
	}

	// --- 2. Throws exception or returning the best match.
	if (best.first == std::numeric_limits<RkUint32>::max())
		return nullptr;

	return &m_family_views[best.first];
}

RkVoid RenderDevice::InitTracyVkContext() noexcept
{
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
	else if (Logger const* logger {m_service_provider.LocateService<Logger>()})
		logger->Warning(service_name, "GPU tracing is unavailable for device named {}", m_name.data());
}

vk::raii::PhysicalDevice RenderDevice::SelectPhysicalDevice() const noexcept
{
	// For now, we will only select the first device
	return m_instance->instance.enumeratePhysicalDevices()[0];
}

std::vector<vk::DeviceQueueCreateInfo> RenderDevice::MakeQueueCreateInfo() const noexcept
{
	// Creating as many queues as there are families
	std::vector const   				   queue_properties  {m_physical_device.getQueueFamilyProperties()};
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos(queue_properties.size());

	static constexpr RkFloat priority {1.0f};
	for (auto&& [index, property] : std::views::enumerate(queue_properties))
	{
		queue_create_infos[index] = vk::DeviceQueueCreateInfo {
			.flags			  = {},
			.queueFamilyIndex = static_cast<RkUint32>(index),
			.queueCount		  = 1,
			.pQueuePriorities = &priority
		};
	}

	return queue_create_infos;
}

RkVoid RenderDevice::FetchQueues() noexcept
{
	m_queues	   .reserve(m_family_views.size());
	m_command_pools.reserve(m_family_views.size());

	for (RkUint32 i = 0; i < m_family_views.size(); i++)
	{
		auto& access = SyncWait(m_family_views[i].AsyncWrite()).Result();
		auto& queue  = m_queues	      .emplace_back(m_device, i, 0);
		auto& pool   = m_command_pools.emplace_back(m_device, vk::CommandPoolCreateInfo {
            .flags			  = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = i
        });

		access->queue        = &queue;
		access->command_pool = &pool;
	}
}
