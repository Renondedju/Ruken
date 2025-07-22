#include "Rendering/Windowing/Window.hpp"
#include "Maths/Vector/PixelVector2.hpp"
#include "Debug/Exception.hpp"

#include <volk.h>
#include <vulkan/vulkan_raii.hpp>
#include <glfw/glfw3.h>

USING_RUKEN_NAMESPACE

Window::Window(RenderDevice& in_device, Vector2px const& in_size, std::string_view const in_name):
	m_owner     {in_device},
	m_window    {[&] { // -- 1. Creating the window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		auto const window = glfwCreateWindow(
			static_cast<int>(in_size.width ),
			static_cast<int>(in_size.height),
			in_name.data(), nullptr, nullptr
		);

		const RkChar* description {nullptr};
		if (glfwGetError(&description) != GLFW_NO_ERROR)
			throw Exception("GLFW Error: " + std::string(description));

		glfwSetWindowUserPointer(window, this);

		return window;
	}()},
	m_surface   {[&] { // -- 2. Creating a surface
		VkSurfaceKHR surface;
		if (vk::Result const result {glfwCreateWindowSurface(*m_owner.GetInstance(), m_window, nullptr, &surface)}; result != vk::Result::eSuccess)
			throw Exception("Could not create window surface : " + vk::to_string(result));

		return vk::raii::SurfaceKHR {m_owner.GetInstance(), surface};
	}()},
	m_swapchain {[&] { // -- 3. Creating the swapchain
		int width, height {};
		glfwGetFramebufferSize(m_window, &width, &height);
		vk::SurfaceCapabilitiesKHR const capabilities {m_owner.GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface)};
		vk::Extent2D			   const extent {
			.width  = std::clamp<uint32_t>(width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width),
			.height = std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};

		return m_owner.GetDevice().createSwapchainKHR(vk::SwapchainCreateInfoKHR {
			.sType                 = vk::StructureType::eSwapchainCreateInfoKHR,
			.pNext                 = nullptr,
			.flags                 = {},
			.surface               = m_surface,
			.minImageCount         = 2,
			.imageFormat	       = vk::Format::eR8G8B8A8Srgb,
			.imageColorSpace       = vk::ColorSpaceKHR::eSrgbNonlinear,
			.imageExtent           = extent,
			.imageArrayLayers      = 1,
			.imageUsage		       = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode      = vk::SharingMode::eExclusive,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices   = nullptr,
			.preTransform   	   = vk::SurfaceTransformFlagBitsKHR::eIdentity,
			.compositeAlpha 	   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode    	   = vk::PresentModeKHR::eFifo,
			.clipped			   = true,
			.oldSwapchain   	   = nullptr,
		});
	}()}
{}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}
