#include "Rendering/Windowing/Window.hpp"
#include "Maths/Vector/PixelVector.hpp"
#include "Debug/Exception.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <glfw/glfw3.h>

USING_RUKEN_NAMESPACE

Window::Window(ServiceProvider& in_service_provider, Vector2px const& in_initial_size, std::string_view const in_name):
	Service		{in_service_provider, typeid(Window)},
	m_owner     {[&] -> RenderDevice& {

		RenderDevice* device {in_service_provider.LocateService<RenderDevice>()};
		if (!device)
			throw Exception(
				std::format(
					"The window named '{}' could not be initialized because the service "
					"provider named '{}' has not been able to locate a RenderDevice.",
					in_name.data(), in_service_provider.GetName().data()
				)
			);

		return *device;
	}()},
	m_window    {[&] { // -- 1. Creating the window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		auto const window = glfwCreateWindow(
			static_cast<int>(in_initial_size.Width() ),
			static_cast<int>(in_initial_size.Height()),
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
	}()}
{
	// Setting up handlers
	glfwSetFramebufferSizeCallback(m_window, &Window::GLFWFramebufferResizeCallback);
	glfwSetMouseButtonCallback    (m_window, &Window::GLFWMouseButtonCallback);
	glfwSetScrollCallback         (m_window, &Window::GLFWScrollCallback);
	glfwSetCursorPosCallback      (m_window, &Window::GLFWCursorPositionCallback);
	glfwSetCursorEnterCallback    (m_window, &Window::GLFWCursorEnterExitCallback);
	glfwSetKeyCallback            (m_window, &Window::GLFWKeyCallback);
	glfwSetCharCallback           (m_window, &Window::GLFWCharacterCallback);
	glfwSetDropCallback           (m_window, &Window::GLFWDropCallback);

	RecreateSwapchain();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

Vector2px Window::GetSize() const noexcept
{
	int width, height {};
	glfwGetFramebufferSize(m_window, &width, &height);

	return Vector2px {
		static_cast<Pixels>(width), static_cast<Pixels>(height)
	};
}

RkBool Window::ShouldClose() const noexcept
{
	return glfwWindowShouldClose(m_window);
}

GLFWwindow* Window::GLFWHandle() const noexcept
{
	return m_window;
}

RkVoid Window::AddCallbackReceiver(WindowCallbackReceiver* in_callback_receiver) noexcept
{
	m_callback_receivers.emplace_back(in_callback_receiver);
}

RkVoid Window::GLFWFramebufferResizeCallback(GLFWwindow* in_window, int const in_width, int const in_height)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	window->RecreateSwapchain();
	for (auto* receiver : window->m_callback_receivers)
		receiver->FramebufferResizeCallback(*window, in_width, in_height);
}

RkVoid Window::GLFWMouseButtonCallback(GLFWwindow* in_window, int const in_button, int const in_action, int const in_mods)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->MouseButtonCallback(*window, in_button, in_action, in_mods);
}

RkVoid Window::GLFWScrollCallback(GLFWwindow* in_window, double const in_x_offset, double const in_y_offset)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->ScrollCallback(*window, in_x_offset, in_y_offset);
}

RkVoid Window::GLFWCursorPositionCallback(GLFWwindow* in_window, double const in_x_position, double const in_y_position)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->CursorPositionCallback(*window, in_x_position, in_y_position);
}

RkVoid Window::GLFWCursorEnterExitCallback(GLFWwindow* in_window, int const in_entered)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->CursorEnterExitCallback(*window, in_entered);
}

RkVoid Window::GLFWKeyCallback(GLFWwindow* in_window, int const in_key, int const in_scancode, int const in_action, int const in_modifiers)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->KeyCallback(*window, in_key, in_scancode, in_action, in_modifiers);
}

RkVoid Window::GLFWCharacterCallback(GLFWwindow* in_window, unsigned int const in_character)
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->CharacterCallback(*window, in_character);
}

RkVoid Window::GLFWDropCallback(GLFWwindow* in_window, int const in_path_count, const char* in_paths[])
{
	Window* window {static_cast<Window*>(glfwGetWindowUserPointer(in_window))};
	RUKEN_ASSERT(window != nullptr, "The glfw window user pointer should be set.");

	for (auto* receiver : window->m_callback_receivers)
		receiver->DropCallback(*window, in_path_count, in_paths);
}

RkVoid Window::RecreateSwapchain()
{
	int width, height {};
	glfwGetFramebufferSize(m_window, &width, &height);
	vk::SurfaceCapabilitiesKHR const capabilities {m_owner.GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface)};
	vk::Extent2D			   const extent {
		.width  = std::clamp<uint32_t>(width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width),
		.height = std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
	};

	auto formats	   {m_owner.GetPhysicalDevice().getSurfaceFormatsKHR	 (m_surface)};
	auto present_modes {m_owner.GetPhysicalDevice().getSurfacePresentModesKHR(m_surface)};

	ResourcePtr const old_swapchain {m_swapchain.Current()};

	// Exchange is atomic
	// Because of that sub-resources need to all be contained in GPUSwapchainData. Otherwise, consumers might
	// read from the new swapchain and the old image views at the same time.
	m_swapchain.Exchange(std::make_shared<GPUSwapchainData>(m_owner, vk::SwapchainCreateInfoKHR {
		.flags                 = {},
		.surface               = m_surface,
		.minImageCount         = capabilities.minImageCount,
		.imageFormat	       = formats[0].format,
		.imageColorSpace       = formats[0].colorSpace,
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
		.oldSwapchain   	   = old_swapchain ? *old_swapchain->swapchain : nullptr,
	}));
}
