#include "Rendering/VulkanLoaderInstance.hpp"
#include "Debug/Exception.hpp"

#include <volk.h>

USING_RUKEN_NAMESPACE

VulkanLoaderInstance::VulkanLoaderInstance()
{
	if (volkInitialize() != VK_SUCCESS)
		throw Exception("Vulkan loader (volk.h) initialization failed");
}

VulkanLoaderInstance::~VulkanLoaderInstance()
{
	volkFinalize();
}