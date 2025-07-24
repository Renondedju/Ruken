#include "Rendering/Vulkan/VulkanLoaderInstance.hpp"
#include "Debug/Exception.hpp"

#include <volk.h>
#include <glfw/glfw3.h>

USING_RUKEN_NAMESPACE

VulkanLoaderInstance::VulkanLoaderInstance()
{
	if (volkInitialize() != VK_SUCCESS)
		throw Exception("Vulkan loader (volk.h) initialization failed");

	if (glfwInit() != GLFW_TRUE)
		throw Exception("Cannot initialize GLFW.");
}

VulkanLoaderInstance::~VulkanLoaderInstance()
{
	glfwTerminate();
	volkFinalize ();
}