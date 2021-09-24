#pragma once

#include "Rendering/RenderFrame.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Window;
class RenderDevice;
class RenderContext;

class RenderWindow
{
    private:

        #pragma region Members

        Logger* m_logger;
        RenderContext* m_context;
        RenderDevice* m_device;

        vk::SurfaceKHR   m_surface;
        vk::SwapchainKHR m_swapchain;

        RkUint32 m_image_count;
        vk::Format m_image_format;
        vk::ColorSpaceKHR m_color_space;
        vk::Extent2D m_image_extent;
        vk::PresentModeKHR m_present_mode;

        vk::Image m_depth;
        vk::ImageView m_depth_view;
        vk::Allocation m_depth_allocation;

        std::vector<vk::Image> m_images;
        std::vector<vk::ImageView> m_image_views;
        std::vector<vk::Framebuffer> m_framebuffers;

        std::vector<RenderFrame> m_render_frames;

        RkUint32 m_current_frame = 0;
        RkUint32 m_image_index;
        RkBool m_valid;

        #pragma endregion

        RkVoid CreateSurface(Window const& in_window) noexcept;
        RkVoid CreateSwapchain(vk::SwapchainKHR in_old_swapchain = VK_NULL_HANDLE) noexcept;

        RkVoid PickSwapchainImageCount ();
        RkVoid PickSwapchainExtent     (RkInt32 in_width, RkInt32 in_height);
        RkVoid PickSwapchainFormat     ();
        RkVoid PickSwapchainPresentMode();

    public:

        #pragma region Constructors

        RenderWindow(Window& in_window, RenderContext* in_context, RenderDevice* in_device, Logger* in_logger = nullptr) noexcept;

        RenderWindow(RenderWindow const& in_copy) = delete;
        RenderWindow(RenderWindow&&      in_move) = default;

        ~RenderWindow() noexcept;

        #pragma endregion

        #pragma region Methods

        RkUint32 GetImageCount() const noexcept;
        vk::Format GetImageFormat() const noexcept;
        vk::Extent2D GetImageExtent() const noexcept;
        std::vector<vk::Image> const& GetImages() const noexcept;
        std::vector<vk::ImageView> const& GetImageViews() const noexcept;
        RkBool IsValid() const noexcept;

        #pragma endregion

        RkVoid Begin();
        RkVoid Render();
        RkVoid End();

        #pragma region Operators

        RenderWindow& operator=(RenderWindow const& in_copy) = delete;
        RenderWindow& operator=(RenderWindow&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE