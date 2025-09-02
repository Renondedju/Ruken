#pragma once

#include "Core/Service.hpp"
#include "Core/Meta/Meta.hpp"

#include "Rendering/Vulkan/VulkanInstance.hpp"

#include <vulkan/vulkan_raii.hpp>

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
	~RenderDevice() override				     = default;

	#pragma endregion

	#pragma region Methods

	// Getters
	vk::raii::Instance&       GetInstance() const noexcept { return m_instance->instance; }
	vk::raii::PhysicalDevice& GetPhysicalDevice() noexcept { return m_physical_device; }
	vk::raii::Device&		  GetDevice()		  noexcept { return m_device; }

	#pragma endregion

	private:

		#pragma region Members

		VulkanInstance*			 	 		   m_instance;
		vk::raii::PhysicalDevice 	 		   m_physical_device;
		std::vector<RkFloat>		 		   m_queue_priorities;
		std::vector<vk::DeviceQueueCreateInfo> m_queue_create_infos;
		vk::raii::Device		 	 		   m_device;
		std::vector<vk::raii::Queue> 		   m_queues;
		vk::raii::CommandPool				   m_command_pool;

		static inline std::vector<const RkChar*> s_extensions {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
			VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
		};

		#pragma endregion

		#pragma region Methods

		vk::raii::PhysicalDevice			   SelectPhysicalDevice() const noexcept;
		std::vector<vk::DeviceQueueCreateInfo> MakeQueueCreateInfo () const noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE
