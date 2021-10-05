#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Window;
class RenderContext;
class RenderDevice;
class RenderFrame;

class RenderWindow
{
    private:

        #pragma region Members

        Logger*        m_logger;
        RenderContext* m_context;
        RenderDevice*  m_device;

        vk::SurfaceKHR   m_surface;
        vk::SwapchainKHR m_swapchain;

        RkBool             m_valid;
        RkUint32           m_image_index;
        RkUint32           m_image_count;
        vk::Extent2D       m_image_extent;
        vk::Format         m_image_format;
        vk::ColorSpaceKHR  m_color_space;
        vk::PresentModeKHR m_present_mode;

        std::vector<vk::Image>       m_images;
        std::vector<vk::ImageView>   m_image_views;

        #pragma endregion

        RkVoid CreateSurface(Window const& in_window) noexcept;
        RkVoid CreateSwapchain(vk::SwapchainKHR in_old_swapchain = VK_NULL_HANDLE) noexcept;

        RkVoid PickSwapchainImageCount ();
        RkVoid PickSwapchainExtent     (RkInt32 in_width, RkInt32 in_height);
        RkVoid PickSwapchainFormat     ();
        RkVoid PickSwapchainPresentMode();

    public:

        #pragma region Constructors

        RenderWindow(Logger* in_logger, RenderContext* in_context, RenderDevice* in_device, Window& in_window) noexcept;

        RenderWindow(RenderWindow const& in_copy) = delete;
        RenderWindow(RenderWindow&&      in_move) = default;

        ~RenderWindow() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Present(RenderFrame& in_frame) noexcept;

        RkUint32 GetImageCount() const noexcept;
        vk::Format GetImageFormat() const noexcept;
        vk::Extent2D GetImageExtent() const noexcept;
        std::vector<vk::Image> const& GetImages() const noexcept;
        std::vector<vk::ImageView> const& GetImageViews() const noexcept;
        RkBool IsValid() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderWindow& operator=(RenderWindow const& in_copy) = delete;
        RenderWindow& operator=(RenderWindow&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE