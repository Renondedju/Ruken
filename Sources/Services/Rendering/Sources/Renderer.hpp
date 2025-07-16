#pragma once

#include "Build/Namespace.hpp"
#include "Core/Service.hpp"
#include "Meta/Meta.hpp"

#include "Rendering/VulkanInstance.hpp"

#include <volk.h>
#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE


/// @brief A single physical rendering device.
struct Renderer final : Service
{
	// Static name of the service, used by the logger to report service errors
	constexpr static std::string_view service_name = RUKEN_STRING(Renderer);

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_parent Parent provider.
	 */
	explicit Renderer(ServiceProvider& in_parent);
	Renderer		   (const Renderer&) = delete;
	Renderer		   (Renderer&&     ) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&     ) = delete;
	~Renderer() override				 = default;

	#pragma endregion

	private:

		#pragma region Members

		VulkanInstance*			 	 		   m_instance;
		vk::raii::PhysicalDevice 	 		   m_physical_device;
		std::vector<RkFloat>		 		   m_queue_priorities;
		std::vector<vk::DeviceQueueCreateInfo> m_queue_create_infos;

		vk::raii::Device		 	 m_device;
		std::vector<vk::raii::Queue> m_queues;

		#pragma endregion

		#pragma region Methods

		vk::raii::PhysicalDevice			   SelectPhysicalDevice() const noexcept;
		std::vector<vk::DeviceQueueCreateInfo> MakeQueueCreateInfo () const noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE
