#include "Rendering/Windowing/Window.hpp"
#include "Maths/Vector/PixelVector2.hpp"
#include "Debug/Exception.hpp"

#include <volk.h>
#include <vulkan/vulkan_raii.hpp>
#include <glfw/glfw3.h>

USING_RUKEN_NAMESPACE

Window::Window(vk::raii::Instance& in_instance, Vector2px const& in_size, std::string_view const in_name):
	m_vulkan {in_instance}
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(
		static_cast<int>(in_size.width ),
		static_cast<int>(in_size.height),
		in_name.data(), nullptr, nullptr
	);

	const RkChar* description {nullptr};
	if (glfwGetError(&description) != GLFW_NO_ERROR)
		throw Exception("GLFW Error: " + std::string(description));

	glfwSetWindowUserPointer(m_window, this);

	VkSurfaceKHR surface;
	if (vk::Result const result {glfwCreateWindowSurface(*m_vulkan, m_window, nullptr, &surface)}; result != vk::Result::eSuccess)
	 	throw Exception("Could not create window surface : " + vk::to_string(result));

	m_surface = surface;
}

Window::~Window()
{
	vkDestroySurfaceKHR(*m_vulkan, m_surface, nullptr);
	glfwDestroyWindow  (m_window);
}