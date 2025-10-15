#pragma once

#include "Core/Service.hpp"
#include "Core/Meta/Meta.hpp"
#include "Build/BuildInfo.hpp"

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

	// Getters
	vk::raii::Instance&       GetInstance()      const noexcept { return m_instance->instance; }
	vk::raii::PhysicalDevice& GetPhysicalDevice()      noexcept { return m_physical_device; }
	vk::raii::Device&		  GetDevice()		       noexcept { return m_device; }
	vk::raii::CommandBuffer&  GetCommandBuffer()       noexcept { return m_command_buffers[0]; }
	vk::raii::Queue&		  GetQueue()						{ return m_queues.front(); }
	TracyVkCtx				  TracyContext()     const noexcept { return m_tracy_context; }

	#pragma endregion

	private:

		#pragma region Members

		VulkanInstance*			 	 		   m_instance;
		vk::raii::PhysicalDevice 	 		   m_physical_device;
		std::string 						   m_name;
		std::vector<RkFloat>		 		   m_queue_priorities;
		std::vector<vk::DeviceQueueCreateInfo> m_queue_create_infos;
		vk::raii::Device		 	 		   m_device;
		std::vector<vk::raii::Queue> 		   m_queues;
		vk::raii::CommandPool				   m_command_pool;
		vk::raii::CommandBuffers			   m_command_buffers;

		#ifdef RUKEN_TRACE_BUILD

		TracyVkCtx m_tracy_context {};

		#endif

		static inline std::vector<const RkChar*> s_extensions {
			vk::KHRSwapchainExtensionName,
			vk::KHRSpirv14ExtensionName,
			vk::KHRSynchronization2ExtensionName,
			vk::KHRCreateRenderpass2ExtensionName,
			vk::KHRDynamicRenderingExtensionName,
			vk::KHRShaderDrawParametersExtensionName,

			// Debug
			vk::EXTHostQueryResetExtensionName,
			vk::EXTCalibratedTimestampsExtensionName
		};

		#pragma endregion

		#pragma region Methods

		vk::raii::PhysicalDevice			   SelectPhysicalDevice() const noexcept;
		std::vector<vk::DeviceQueueCreateInfo> MakeQueueCreateInfo () const noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE
