#pragma once

#include <GLFW/glfw3.h>

#include "Build/Namespace.hpp"
#include "Maths/Vector/PixelVector2.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief A window on a desktop environment.
struct Window
{
	Window(Window const&) 			 = delete;
	Window(Window&&     ) 			 = delete;
	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&     ) = delete;
	~Window();

	protected:

		friend struct WindowManager;

		GLFWwindow* m_window;

		/**
		 * Default constructor.
		 * @param in_size Initial size of the window.
		 * @param in_name Initial title of the window.
		 */
		explicit Window(Vector2px const& in_size, std::string_view in_name);
};

END_RUKEN_NAMESPACE