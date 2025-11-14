#pragma once

#include "Core/Service.hpp"
#include "Core/Meta/Meta.hpp"
#include "Core/JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "Rendering/FamilyView.hpp"
#include "Rendering/Vulkan/VulkanInstance.hpp"

#include <string>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <tracy/TracyVulkan.hpp>

BEGIN_RUKEN_NAMESPACE

/// @brief A single physical rendering device.
struct RenderDevice final : Service
{
	// Static name of the service, used by the logger to report service errors
	constexpr static std::string_view service_name = RUKEN_STRING(Renderer);

	#pragma region Lifetime

	/**
	 * Constructor.
	 * TODO: Find a way to select the underlying VkPhysicalDevice.
	 * @param in_parent Parent provider.
	 */
	explicit RenderDevice(ServiceProvider& in_parent);
	RenderDevice		   (const RenderDevice&) = delete;
	RenderDevice		   (RenderDevice&&     ) = delete;
	RenderDevice& operator=(const RenderDevice&) = delete;
	RenderDevice& operator=(RenderDevice&&     ) = delete;
	~RenderDevice() override;

	#pragma endregion

	#pragma region Methods

	// --- Device & Instance
	vk::raii::Instance&       GetInstance()  const noexcept;
	vk::raii::PhysicalDevice& GetPhysicalDevice()  noexcept;
	vk::raii::Device&		  GetDevice()		   noexcept;
	TracyVkCtx				  TracyContext() const noexcept;

	/**
	 * Returns the best suited queue family supporting every passed flags.
	 * If no family is compatible, nullptr will be returned instead.
	 *
	 * @param in_queue_flags Requested queue flags.
	 * @return Pointer to a mutex protecting a vulkan queue family.
	 */
	SharedMutex<FamilyView>* FindQueueFamily(vk::QueueFlags in_queue_flags);

	#pragma endregion

	private:

		#pragma region Members

		// --- Device & Instance
		VulkanInstance*			 m_instance;
		vk::raii::PhysicalDevice m_physical_device;
		vk::raii::Device		 m_device;
		std::string 			 m_name;

		static inline std::vector<char const*> s_extensions {
			vk::KHRSwapchainExtensionName,				// Presentation capability
			vk::EXTSwapchainMaintenance1ExtensionName, // Allows to wait for presentation to end before deleting synchro

			vk::KHRSpirv14ExtensionName,
			vk::KHRSynchronization2ExtensionName,
			vk::KHRDynamicRenderingExtensionName,

			// Debug
			vk::EXTHostQueryResetExtensionName,
			vk::EXTCalibratedTimestampsExtensionName
		};

		// --- Queues
		std::vector<vk::raii::Queue>	     m_queues		 {};
		std::vector<vk::raii::CommandPool>   m_command_pools {};
		std::vector<SharedMutex<FamilyView>> m_family_views;

		// --- Instrumentation
		TracyVkCtx m_tracy_context {};

		#pragma endregion

		#pragma region Methods

		/// @brief Checks for support and inits tracy vulkan context.
		RkVoid InitTracyVkContext() noexcept;

		vk::raii::PhysicalDevice			   SelectPhysicalDevice() const noexcept;
		std::vector<vk::DeviceQueueCreateInfo> MakeQueueCreateInfo () const noexcept;
		RkVoid								   FetchQueues		   () noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE
