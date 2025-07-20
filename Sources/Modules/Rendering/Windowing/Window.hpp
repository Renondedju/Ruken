#pragma once

#include "Maths/Vector/PixelVector2.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <glfw/glfw3.h>

BEGIN_RUKEN_NAMESPACE

/// @brief A window on a desktop environment.
struct Window
{
	/**
	 * Default constructor.
	 * @param in_instance Vulkan instance.
	 * @param in_size Initial size of the window.
	 * @param in_name Initial title of the window.
	 */
	explicit Window(vk::raii::Instance& in_instance, Vector2px const& in_size, std::string_view in_name);
	Window(Window const&) 			 = delete;
	Window(Window&&     ) 			 = delete;
	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&     ) = delete;
	~Window();

	protected:

		friend struct WindowManager;

		vk::raii::Instance& m_vulkan;
		GLFWwindow*	 		m_window  {};
		vk::SurfaceKHR		m_surface {};
};

END_RUKEN_NAMESPACE