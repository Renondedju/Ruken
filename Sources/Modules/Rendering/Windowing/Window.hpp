#pragma once

#include "Resources/ResourceHandle.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/Resources/GPUSwapchain.hpp"
#include "Core/Maths/Vector/PixelVector.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <glfw/glfw3.h>

BEGIN_RUKEN_NAMESPACE

/// @brief A window on a desktop environment.
struct Window
{
	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_device Owning device.
	 * @param in_size Initial size of the window.
	 * @param in_name Initial title of the window.
	 */
	explicit Window(RenderDevice& in_device, Vector2px const& in_size, std::string_view in_name);
	Window(Window const&) 			 = delete;
	Window(Window&&     ) 			 = delete;
	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&     ) = delete;
	~Window();

	#pragma endregion

	#pragma region Methods

	vk::raii::SurfaceKHR& GetSurface  () noexcept { return m_surface; }
	GPUSwapchain&		  GetSwapchain() noexcept { return m_swapchain; }
	vk::Extent2D		  GetExtent   () const noexcept;
	RkBool				  ShouldClose () const noexcept;

	#pragma endregion

	private:

		#pragma region Members

		RenderDevice&        m_owner;
		GLFWwindow*	         m_window;
		vk::raii::SurfaceKHR m_surface;
		GPUSwapchain	     m_swapchain;

		#pragma endregion

		#pragma region Methods

		static RkVoid GLFWFramebufferResizeCallback(GLFWwindow* window, int width, int height) noexcept;

		RkVoid RecreateSwapchain();

		#pragma endregion
};

END_RUKEN_NAMESPACE