#pragma once

#include "Rendering/Vulkan/VulkanLoaderInstance.hpp"

#include "Types/FundamentalTypes.hpp"
#include "Core/Service.hpp"
#include "Meta/Meta.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE


/// @brief Stores and manages the project-wide Vulkan instance.
struct VulkanInstance final: Service
{
	/// @brief Pretty name of the service.
	constexpr static std::string_view service_name = RUKEN_STRING(VulkanInstance);

	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_parent Parent provider.
	 * @param in_layers List of vulkan layers to enable.
	 * @param in_extensions List of instance extensions to enable.
	 */
	explicit VulkanInstance(ServiceProvider& in_parent,
		std::vector<const RkChar*> in_layers,
		std::vector<const RkChar*> in_extensions);

	VulkanInstance           (const VulkanInstance&) = delete;
	VulkanInstance           (VulkanInstance&&     ) = delete;
	VulkanInstance& operator=(const VulkanInstance&) = delete;
	VulkanInstance& operator=(VulkanInstance&&     ) = delete;
	~VulkanInstance() override;

	#pragma endregion

	VulkanLoaderInstance 	 loader  {};
	vk::raii::Context    	 context {};
	vk::ApplicationInfo		 app_info;
	vk::raii::Instance   	 instance;
	VkDebugUtilsMessengerEXT debug_utils_messenger {nullptr};

	private:

		#pragma region Methods

		/**
		 * Fills in and returns a vk::InstanceCreateInfo instance.
		 * @param in_service_provider Service provider.
		 * @param in_layers Instance layers to enable.
		 * @param in_extensions Instance extensions to enable.
		 * @return vk::InstanceCreateInfo.
		 */
		vk::InstanceCreateInfo MakeInstanceCreateInfo(
			ServiceProvider const&		    in_service_provider,
			std::vector<const RkChar*>& in_layers,
			std::vector<const RkChar*>& in_extensions
		) const;

		/// @brief Checks if all the passed layers are available, throws an exception otherwise.
		RkVoid ValidateLayers    (std::vector<const RkChar*> const& in_layers    ) const;
		RkVoid ValidateExtensions(std::vector<const RkChar*> const& in_extensions) const;

		/**
		 * Logs a vulkan debug callback.
		 * @param in_message_severity Message severity.
		 * @param in_message_type Message type.
		 * @param in_callback_data Message details.
		 * @param in_user_data Pointer to the parent Logger.
		 * @return The application should always return VK_FALSE.
		 *		   The VK_TRUE value is reserved for use in layer development.
		 */
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT	  	in_message_severity,
			VkDebugUtilsMessageTypeFlagsEXT			  	in_message_type,
			const VkDebugUtilsMessengerCallbackDataEXT* in_callback_data,
			void*										in_user_data) noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE