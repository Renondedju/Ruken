#include "Windowing/Window.hpp"
#include "Maths/Vector/PixelVector2.hpp"

#include <glfw/glfw3.h>

#include "Core/Exception.hpp"

USING_RUKEN_NAMESPACE

Window::Window(Vector2px const& in_size, std::string_view const in_name):
	m_window {glfwCreateWindow(
		static_cast<int>(in_size.width ),
		static_cast<int>(in_size.height),
		in_name.data(), nullptr, nullptr
	)}
{
	const RkChar* description {nullptr};
	if (glfwGetError(&description) != GLFW_NO_ERROR)
		throw Exception("GLFW Error: " + std::string(description));

	glfwSetWindowUserPointer(m_window, this);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}