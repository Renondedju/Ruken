#include "Build/ProjectInfo.hpp"
#include "Debug/Exception.hpp"
#include "Debug/Logging/Logger.hpp"
#include "Rendering/Vulkan/VulkanInstance.hpp"

#define VOLK_IMPLEMENTATION

#include <volk.h>
#include <glfw/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_raii.hpp>

USING_RUKEN_NAMESPACE

VulkanInstance::VulkanInstance(
	ServiceProvider&		   in_parent,
	std::vector<const RkChar*> in_layers,
	std::vector<const RkChar*> in_extensions):
	Service    {in_parent, typeid(VulkanInstance)},
	app_info   { // TODO: Extend the ProjectInfo class with versions
		.pApplicationName   = ProjectInfo::ProjectName,
		.applicationVersion = 0u,
		.pEngineName   		= ProjectInfo::EngineName,
		.engineVersion 		= 0u,
		.apiVersion			= VK_API_VERSION_1_4
	},
	instance   {context, MakeInstanceCreateInfo(in_parent, in_layers, in_extensions)}
{
	volkLoadInstance(*instance);

	if (Logger* logger {in_parent.LocateService<Logger>()})
	{
		VkDebugUtilsMessengerCreateInfoEXT const create_info {
			.sType 			 = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext 			 = nullptr,
			.flags			 = {},
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType	 = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = &VulkanInstance::DebugCallback,
			.pUserData		 = logger
		};

		if (vkCreateDebugUtilsMessengerEXT(*instance, &create_info, nullptr, &debug_utils_messenger) != VK_SUCCESS)
			throw Exception("Could not setup the Vulkan debug utils messenger");
	}
}

VulkanInstance::~VulkanInstance()
{
	if (debug_utils_messenger)
		vkDestroyDebugUtilsMessengerEXT(*instance, debug_utils_messenger, nullptr);
}

vk::InstanceCreateInfo VulkanInstance::MakeInstanceCreateInfo(
	ServiceProvider const&		in_service_provider,
	std::vector<const RkChar*>& in_layers,
	std::vector<const RkChar*>& in_extensions) const
{
	if constexpr (BuildInfo::HasDebugInfo)
		in_layers.emplace_back("VK_LAYER_KHRONOS_validation");

	// Presentation support
	std::uint32_t extension_count {};
	const RkChar** required_ext   {glfwGetRequiredInstanceExtensions(&extension_count)};
	for (std::uint32_t i = 0; i < extension_count; ++i)
		in_extensions.emplace_back(required_ext[i]);

	// Logging
	if (auto const logger = in_service_provider.LocateService<Logger>())
	{
		in_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		logger->Info(service_name, "Enabling the following Vulkan extensions:");
		for (auto const& extension : in_extensions)
			logger->Info(service_name, "\t{}", extension);
	}

	// Throwing if something is unavailable.
	ValidateLayers	  (in_layers);
	ValidateExtensions(in_extensions);

	return vk::InstanceCreateInfo {
		.pApplicationInfo        = &app_info,
		.enabledLayerCount       = static_cast<uint32_t>(in_layers.size()),
		.ppEnabledLayerNames     = in_layers.data(),
		.enabledExtensionCount   = static_cast<uint32_t>(in_extensions.size()),
		.ppEnabledExtensionNames = in_extensions.data()
	};
}

RkVoid VulkanInstance::ValidateLayers(std::vector<const RkChar*> const& in_layers) const
{
	auto const available_layers {context.enumerateInstanceLayerProperties()};
	for (auto const requested_layer : in_layers)
	{
		bool layer_found {false};
		for (auto available_layer : available_layers)
			if (std::strcmp(requested_layer, available_layer.layerName) == 0)
			{ layer_found = true; break; }

		if (!layer_found)
			throw Exception(std::format("Requested vulkan layer is not available: {}", requested_layer));
	}
}

RkVoid VulkanInstance::ValidateExtensions(std::vector<const RkChar*> const& in_extensions) const
{
	auto const available_extensions {context.enumerateInstanceExtensionProperties()};
	for (auto const requested_extension : in_extensions)
	{
		bool extension_found {false};
		for (auto available_extension : available_extensions)
			if (std::strcmp(requested_extension, available_extension.extensionName) == 0)
			{ extension_found = true; break; }

		if (!extension_found)
			throw Exception(std::format("Requested vulkan extension is not available: {}", requested_extension));
	}
}

VkBool32 VulkanInstance::DebugCallback(
					 VkDebugUtilsMessageSeverityFlagBitsEXT const in_message_severity,
	[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT		 	  in_message_type,
					 const VkDebugUtilsMessengerCallbackDataEXT*  in_callback_data,
					 void*										  in_user_data) noexcept
{
	Logger const* logger {static_cast<Logger*>(in_user_data)};

	if (!logger)
		return VK_FALSE;

	switch (in_message_severity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			logger->Debug  (service_name, "{}", in_callback_data->pMessage); return VK_FALSE;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			logger->Info   (service_name, "{}", in_callback_data->pMessage); return VK_FALSE;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			logger->Warning(service_name, "{}", in_callback_data->pMessage); return VK_FALSE;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			logger->Error  (service_name, "{}", in_callback_data->pMessage); return VK_FALSE;

		default:
			std::unreachable();
	}

	return VK_FALSE;
}
