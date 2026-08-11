#pragma once

#include "Core/Maths/Vector/PixelVector.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Resources/GPUSwapchain.hpp"
#include "Rendering/Windowing/WindowCallbackReceiver.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <glfw/glfw3.h>

BEGIN_RUKEN_NAMESPACE
	/// @brief A window on a desktop environment.
/// Services cannot be individually destroyed. Instead the whole provider should be.
/// Since windows can be closed, that means you should plan the destruction of the associated provider accordingly.
struct Window: Service
{
	constexpr static std::string_view service_name = "Window";

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_service_provider Owner.
	 * @param in_initial_size Initial size of the window.
	 * @param in_name Initial title of the window.
	 */
	explicit Window(ServiceProvider& in_service_provider, Vector2px const& in_initial_size, std::string_view in_name);
	Window(Window const&) 			 = delete;
	Window(Window&&     ) 			 = delete;
	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&     ) = delete;
	~Window() override;

	#pragma endregion

	#pragma region Methods

	vk::raii::SurfaceKHR& GetSurface  ()       noexcept { return m_surface; }
	GPUSwapchain&		  GetSwapchain()       noexcept { return m_swapchain; }
	Vector2px			  GetSize	  () const noexcept;
	RkBool				  ShouldClose () const noexcept;
	GLFWwindow*			  GLFWHandle  () const noexcept;

	RkVoid AddCallbackReceiver(WindowCallbackReceiver* in_callback_receiver) noexcept;

	#pragma endregion

	private:

		#pragma region Members

		RenderDevice&        				 m_owner;
		GLFWwindow*	         				 m_window;
		vk::raii::SurfaceKHR 				 m_surface;
		GPUSwapchain	     				 m_swapchain;
		std::vector<WindowCallbackReceiver*> m_callback_receivers;

		#pragma endregion

		#pragma region Methods

		static RkVoid GLFWFramebufferResizeCallback(GLFWwindow* in_window, int in_width,  int in_height);
		static RkVoid GLFWMouseButtonCallback	   (GLFWwindow* in_window, int in_button, int in_action, int in_mods);
		static RkVoid GLFWScrollCallback		   (GLFWwindow* in_window, double in_x_offset  , double in_y_offset);
		static RkVoid GLFWCursorPositionCallback   (GLFWwindow* in_window, double in_x_position, double in_y_position);
		static RkVoid GLFWCursorEnterExitCallback  (GLFWwindow* in_window, int in_entered);
		static RkVoid GLFWKeyCallback			   (GLFWwindow* in_window, int in_key, int in_scancode, int in_action, int in_modifiers);
		static RkVoid GLFWCharacterCallback		   (GLFWwindow* in_window, unsigned int in_character);
		static RkVoid GLFWDropCallback			   (GLFWwindow* in_window, int in_path_count, const char* in_paths[]);

		RkVoid RecreateSwapchain();

		#pragma endregion
};

END_RUKEN_NAMESPACE