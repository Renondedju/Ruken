
#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include "Functional/Event.hpp"

#include "Windowing/WindowParams.hpp"

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderContext;
class RenderDevice;
class RenderFrame;

/**
 * \brief Manages a GLFW window.
 */
class Window
{
    /**
     * \brief WindowManager is the only class able to can create a Window.
     */
    friend class WindowManager;

    private:

        #pragma region Members

        Logger*        m_logger  {nullptr};
        RenderContext* m_context {nullptr};
        RenderDevice*  m_device  {nullptr};

        GLFWwindow* m_handle {nullptr};
        std::string m_name   {};

        vk::SurfaceKHR     m_surface;
        vk::SwapchainKHR   m_swapchain;
        RkUint32           m_image_index;
        RkUint32           m_image_count;
        vk::Extent2D       m_image_extent;
        vk::Format         m_image_format;
        vk::ColorSpaceKHR  m_color_space;
        vk::PresentModeKHR m_present_mode;

        std::vector<vk::Image> m_images;

        #pragma endregion

        #pragma region Constructors

        explicit Window(Logger* in_logger, RenderContext* in_context, RenderDevice* in_device, WindowParams const& in_params) noexcept;

        #pragma endregion

        #pragma region Methods

        #pragma region Callbacks
        
        static RkVoid WindowPosCallback         (GLFWwindow* in_window, RkInt32 in_x_pos, RkInt32 in_y_pos)     noexcept;
        static RkVoid WindowSizeCallback        (GLFWwindow* in_window, RkInt32 in_width, RkInt32 in_height)    noexcept;
        static RkVoid WindowCloseCallback       (GLFWwindow* in_window)                                           noexcept;
        static RkVoid WindowRefreshCallback     (GLFWwindow* in_window)                                           noexcept;
        static RkVoid WindowFocusCallback       (GLFWwindow* in_window, RkInt32 in_focused)                      noexcept;
        static RkVoid WindowIconifyCallback     (GLFWwindow* in_window, RkInt32 in_iconified)                    noexcept;
        static RkVoid WindowMaximizeCallback    (GLFWwindow* in_window, RkInt32 in_maximized)                    noexcept;
        static RkVoid FramebufferSizeCallback   (GLFWwindow* in_window, RkInt32 in_width,   RkInt32 in_height)  noexcept;
        static RkVoid WindowContentScaleCallback(GLFWwindow* in_window, RkFloat in_x_scale, RkFloat in_y_scale) noexcept;

        #pragma endregion

        static Window* GetWindowUserPointer(GLFWwindow* in_window) noexcept;
        
        RkVoid CreateWindow   (WindowParams const& in_params)                      noexcept;
        RkVoid CreateSurface  ()                                                   noexcept;
        RkVoid CreateSwapchain(vk::SwapchainKHR in_old_swapchain = VK_NULL_HANDLE) noexcept;
        RkVoid SetupCallbacks ()                                                   noexcept;

        RkVoid PickSwapchainImageCount ();
        RkVoid PickSwapchainExtent     ();
        RkVoid PickSwapchainFormat     ();
        RkVoid PickSwapchainPresentMode();

        #pragma endregion

    public:

        #pragma region Events

        Event<> on_closed    {};
        Event<> on_refreshed {};

        /**
         * \brief Event raised with the new coordinates, in screen coordinates, of the upper-left corner of the content area of the window.
         */
        Event<RkInt32, RkInt32> on_moved {};

        /**
         * \brief Event raised with the new extent, in screen coordinates, of the window.
         */
        Event<RkInt32, RkInt32> on_resized {};

        /**
         * \brief Event raised with True if the window was given input focus, or False if it lost it.
         */
        Event<RkBool> on_focused {};

        /**
         * \brief Event raised with True if the window was iconified, or False if it was restored.
         */
        Event<RkBool> on_iconified {};

        /**
         * \brief Event raised with True if the window was maximized, or False if it was restored.
         */
        Event<RkBool> on_maximized {};

        /**
         * \brief Event raised with the new extent, in pixels, of the framebuffer.
         */
        Event<RkInt32, RkInt32> on_framebuffer_resized {};

        /**
         * \brief Event raised with the new axis content scale of the window
         */
        Event<RkFloat, RkFloat> on_content_rescaled {};

        #pragma endregion

        #pragma region Constructors

        Window() = delete;

        Window(Window const& in_copy) = delete;
        Window(Window&&      in_move) = delete;

        ~Window() noexcept;

        #pragma endregion

        #pragma region Methods

        #pragma region Setters

        /**
         * \brief This function sets the window title, encoded as UTF-8, of the specified window.
         */
        RkVoid SetName(RkChar const* in_name) noexcept;

        /**
         * \brief Sets the position, in screen coordinates, of the upper-left corner of the content area of the window.
         * \note  If the window is a full screen window, this function does nothing.
         */
        RkVoid SetPosition(Position2D const& in_position) const noexcept;

        /**
         * \brief Sets the size limits of the content area of the window.
         * \param in_min_size The minimum size, in screen coordinates, of the content area.
         * \param in_max_size The maximum size, in screen coordinates, of the content area.
         * \note  If the window is full screen, the size limits only take effect once it is made windowed.
         *        If the window is not resizable, this function does nothing.
         *        The size limits are applied immediately to a windowed mode window and may cause it to be resized.
         */
        RkVoid SetSizeLimits(Extent2D const& in_min_size, Extent2D const& in_max_size) const noexcept;

        /**
         * \brief Sets the required aspect ratio of the content area of the window.
         *        The aspect ratio is specified as a numerator and a denominator and both values must be greater than zero.
         *        For example, the common 16:9 aspect ratio is specified as 16 and 9, respectively.
         * \note  If the window is full screen, the aspect ratio only takes effect once it is made windowed.
         *        If the window is not resizable, this function does nothing.
         */
        RkVoid SetAspectRatio(RkInt32 in_numerator, RkInt32 in_denominator) const noexcept;

        /**
         * \brief Sets the size, in screen coordinates, of the content area of the window.
         * \note  For full screen windows, this function updates the resolution of its desired video mode
         *        and switches to the video mode closest to it, without affecting the window's context.
         */
        RkVoid SetSize(Extent2D const& in_size) const noexcept;

        /**
         * \brief Sets the opacity of the window, including any decorations.
         *        The opacity (or alpha) value is a positive finite number between zero and one, where zero is fully transparent and one is fully opaque.
         * \note  A window created with framebuffer transparency may not use whole window transparency.
         *        The results of doing this are undefined.
         */
        RkVoid SetOpacity(RkFloat in_opacity) const noexcept;

        /**
         * \brief Iconifies (minimizes) the window if it was previously restored.
         *        If the window is already iconified, this function does nothing.
         * \note  If the specified window is a full screen window,
         *        the original monitor resolution is restored until the window is restored.
         */
        RkVoid Iconify() const noexcept;

        /**
         * \brief Restores the window if it was previously iconified (minimized) or maximized.
         *        If the window is already restored, this function does nothing.
         * \note  If the specified window is a full screen window,
         *        the resolution chosen for the window is restored on the selected monitor.
         */
        RkVoid Restore() const noexcept;

        /**
         * \brief Maximizes the window if it was previously not maximized.
         *        If the window is already maximized, this function does nothing.
         * \note  If the window is a full screen window, this function does nothing.
         */
        RkVoid Maximize() const noexcept;

        /**
         * \brief Makes the window visible if it was previously hidden.
         *        If the window is already visible or is in full screen mode, this function does nothing.
         * \note  By default, windowed mode windows are focused when shown.
         *        Set the 'focused_on_show' parameter to change this behavior for a newly created window.
         */
        RkVoid Show() const noexcept;

        /**
         * \brief Hides the window if it was previously visible.
         *        If the window is already hidden or is in full screen mode, this function does nothing.
         */
        RkVoid Hide() const noexcept;

        /**
         * \brief Brings the window to front and sets input focus. The window should already be visible and not iconified.
         * \note  By default, both windowed and full screen mode windows are focused when initially created.
         *        Also by default, windowed mode windows are focused when shown.
         */
        RkVoid Focus() const noexcept;

        /**
         * \brief Requests user attention to the window.
         * \note  On platforms where this is not supported, attention is requested to the application as a whole.
         */
        RkVoid RequestAttention() const noexcept;

        /**
         * \brief Makes the window full screen or windowed.
         * \note  When a window transitions from full screen to windowed mode, this function restores any previous window settings
         *        such as whether it is decorated, floating, resizable, has size or aspect ratio limits, etc.
         */
        RkVoid SetFullscreen(RkBool in_fullscreen) const noexcept;

        /**
         * \brief Sets whether the specified window has decorations.
         */
        [[nodiscard]]
        RkVoid SetDecorated(RkBool in_decorated) const noexcept;

        /**
         * \brief Sets whether the specified window is resizable by the user.
         */
        [[nodiscard]]
        RkVoid SetResizable(RkBool in_resizable) const noexcept;

        /**
         * \brief Sets whether the specified window is floating, also called topmost or always-on-top..
         */
        [[nodiscard]]
        RkVoid SetFloating(RkBool in_floating) const noexcept;

        /**
         * \brief Sets whether the specified full screen window is iconified on focus loss.
         */
        [[nodiscard]]
        RkVoid SetAutoIconified(RkBool in_auto_iconified) const noexcept;

        /**
         * \brief Sets whether the window will be given input focus when shown.
         */
        [[nodiscard]]
        RkVoid SetFocusedOnShow(RkBool in_focused_on_show) const noexcept;

        #pragma endregion

        #pragma region Getters

        [[nodiscard]]
        GLFWwindow* GetHandle() const noexcept;

        [[nodiscard]]
        std::string const& GetName() const noexcept;

        /**
         * \return The value of the close flag of the window. 
         */
        [[nodiscard]]
        RkBool ShouldClose() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the content area of the window.
         */
        [[nodiscard]]
        Position2D GetPosition() const noexcept;

        /**
         * \return The size, in screen coordinates, of the content area of the window.
         */
        [[nodiscard]]
        Extent2D GetSize() const noexcept;

        /**
         * \return The size, in pixels, of the framebuffer of the window.
         */
        [[nodiscard]]
        Extent2D GetFramebufferSize() const noexcept;

        /**
         * \return The size, in screen coordinates, of each edge of the frame of the window.
         * \note   This size includes the title bar, if the window has one.
         */
        [[nodiscard]]
        Rect2D GetFrameSize() const noexcept;

        /**
         * \return The content scale for the window (the ratio between the current DPI and the platform's default DPI).
         * \note   On systems where each monitors can have its own content scale, the window content scale will depend
         *         on which monitor the system considers the window to be on.
         */
        [[nodiscard]]
        Scale2D GetContentScale() const noexcept;

        /**
         * \return The opacity of the window, including any decorations (between 0 and 1).
         * \note   If the system does not support whole window transparency, this function always returns one.
         */
        [[nodiscard]]
        RkFloat GetOpacity() const noexcept;

        /**
         * \return True if the window is in full screen mode, else False.
         */
        [[nodiscard]]
        RkBool IsFullscreen() const noexcept;

        /**
         * \return True if the window has input focus, else False.
         */
        [[nodiscard]]
        RkBool IsFocused() const noexcept;

        /**
         * \return True if the window is iconified, else False.
         */
        [[nodiscard]]
        RkBool IsIconified() const noexcept;

        /**
         * \return True if the window is maximized, else False.
         */
        [[nodiscard]]
        RkBool IsMaximized() const noexcept;

        /**
         * \return True if the cursor is currently directly over the content area of the window, with no other windows between, else False.
         */
        [[nodiscard]]
        RkBool IsHovered() const noexcept;

        /**
         * \return True if the window is visible, else False.
         */
        [[nodiscard]]
        RkBool IsVisible() const noexcept;

        /**
         * \return True if the window is resizable by the user, else False.
         */
        [[nodiscard]]
        RkBool IsResizable() const noexcept;

        /**
         * \return True if the window has decorations (such as a border, a close widget, etc.), else eFalse
         */
        [[nodiscard]]
        RkBool IsDecorated() const noexcept;

        /**
         * \return True if the full screen window is iconified on focus loss, else False.
         */
        [[nodiscard]]
        RkBool IsAutoIconified() const noexcept;

        /**
         * \return True if the window is floating (also called topmost or always-on-top), else False.
         */
        [[nodiscard]]
        RkBool IsFloating() const noexcept;

        /**
         * \return True if the window has a transparent framebuffer, else False.
         */
        [[nodiscard]]
        RkBool IsFramebufferTransparent() const noexcept;

        /**
         * \return True if the window will be given input focus when shown, else False.
         */
        [[nodiscard]]
        RkBool IsFocusedOnShow() const noexcept;

        #pragma endregion

        RkVoid Present(RenderFrame& in_frame) noexcept;

        #pragma endregion

        #pragma region Operators

        Window& operator=(Window const& in_copy) = delete;
        Window& operator=(Window&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE