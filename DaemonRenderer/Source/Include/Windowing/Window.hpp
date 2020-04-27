/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#define GLFW_INCLUDE_NONE

#include <glfw/glfw3.h>

#include "Vector/Vector.hpp"

#include "Functional/Event.hpp"

#include "Windowing/WindowParams.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Manages a GLFW window.
 */
class Window
{
    /**
     * \brief WindowManager is the only class who can create a Window.
     */
    friend class WindowManager;

    private:

        #pragma region Members

        GLFWwindow* m_handle;
        std::string m_name;

        #pragma endregion

        #pragma region Constructors


        /**
         * \brief Creates a window with the specified parameters.
         */

        #pragma endregion

        #pragma region Methods

        #pragma region Callbacks
        
        /**
         * \param in_window The window that was moved.
         * \param in_x_pos  The new x-coordinate, in screen coordinates, of the upper-left corner of the content area of the window.
         * \param in_y_pos  The new y-coordinate, in screen coordinates, of the upper-left corner of the content area of the window.
         */
        static DAEvoid WindowPosCallback(GLFWwindow* in_window, DAEint32 in_x_pos, DAEint32 in_y_pos) noexcept;

        /**
         * \param in_window The window that was resized.
         * \param in_width  The new width, in screen coordinates, of the window.
         * \param in_height The new height, in screen coordinates, of the window.
         */
        static DAEvoid WindowSizeCallback(GLFWwindow* in_window, DAEint32 in_width, DAEint32 in_height) noexcept;

        /**
         * \param in_window The window that the user attempted to close.
         */
        static DAEvoid WindowCloseCallback(GLFWwindow* in_window) noexcept;

        /**
         * \param in_window The window whose content needs to be refreshed.
         */
        static DAEvoid WindowRefreshCallback(GLFWwindow* in_window) noexcept;

        /**
         * \param in_window  The window that gained or lost input focus.
         * \param in_focused 'GLFW_TRUE' if the window was given input focus, or 'GLFW_FALSE' if it lost it.
         */
        static DAEvoid WindowFocusCallback(GLFWwindow* in_window, DAEint32 in_focused) noexcept;

        /**
         * \param in_window    The window that was iconified or restored.
         * \param in_iconified 'GLFW_TRUE' if the window was iconified, or 'GLFW_FALSE' if it was restored.
         */
        static DAEvoid WindowIconifyCallback(GLFWwindow* in_window, DAEint32 in_iconified) noexcept;

        /**
         * \param in_window    The window that was maximized or restored.
         * \param in_maximized 'GLFW_TRUE' if the window was maximized, or 'GLFW_FALSE' if it was restored.
         */
        static DAEvoid WindowMaximizeCallback(GLFWwindow* in_window, DAEint32 in_maximized) noexcept;

        /**
         * \param in_window The window whose framebuffer was resized.
         * \param in_width  The new width, in pixels, of the framebuffer.
         * \param in_height The new height, in pixels, of the framebuffer.
         */
        static DAEvoid FramebufferSizeCallback(GLFWwindow* in_window, DAEint32 in_width, DAEint32 in_height) noexcept;

        /**
         * \param in_window  The window whose content scale changed.
         * \param in_x_scale The new x-axis content scale of the window.
         * \param in_y_scale The new y-axis content scale of the window.
         */
        static DAEvoid WindowContentScaleCallback(GLFWwindow* in_window, DAEfloat in_x_scale, DAEfloat in_y_scale) noexcept;

        #pragma endregion

        /**
         * \return The current value of the user-defined pointer of the specified window.
         */
        static Window* GetWindowUserPointer(GLFWwindow* in_window) noexcept;
        
        /**
         * \brief Creates a window with the specified parameters.
         */
        DAEvoid SetupWindow(WindowParams const& in_params) noexcept;

        /**
         * \brief Sets the callbacks for the newly created window.
         */
        DAEvoid SetupCallbacks() const noexcept;

        #pragma endregion

    public:

        #pragma region Events

        Event<DAEint32, DAEint32>   on_moved;
        Event<DAEint32, DAEint32>   on_resized;
        Event<>                     on_closed;
        Event<>                     on_refreshed;
        Event<DAEbool>              on_focused;
        Event<DAEbool>              on_iconified;
        Event<DAEbool>              on_maximized;
        Event<DAEint32, DAEint32>   on_framebuffer_resized;
        Event<DAEfloat, DAEfloat>   on_content_rescaled;

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit Window(WindowParams const& in_params) noexcept;

        Window(Window const&    in_copy) = delete;
        Window(Window&&         in_move) = default;

        ~Window() noexcept;

        #pragma endregion

        #pragma region Methods

        #pragma region Setters

        /**
         * \brief This can be used to override the user's attempt to close this window,
         *        or to signal that it should be closed.
         */
        DAEvoid SetShouldClose(DAEbool in_should_close) const noexcept;

        /**
         * \brief Sets the title, encoded as UTF-8, of this window.
         */
        DAEvoid SetName(DAEchar const* in_name);

        /**
         * \brief Sets the position, in screen coordinates, of the upper-left corner of the content area of the window.
         * \note  If the window is a full screen window, this function does nothing.
         */
        DAEvoid SetPosition(VkOffset2D const& in_position) const noexcept;

        /**
         * \brief Sets the size limits of the content area of the window.
         * \param in_min_size The minimum size, in screen coordinates, of the content area.
         * \param in_max_size The maximum size, in screen coordinates, of the content area.
         * \note  If the window is full screen, the size limits only take effect once it is made windowed.
         *        If the window is not resizable, this function does nothing.
         */
        DAEvoid SetSizeLimits(VkExtent2D const& in_min_size,
                              VkExtent2D const& in_max_size) const noexcept;

        /**
         * \brief Sets the required aspect ratio of the content area of the window.
         * \note  If the window is full screen, the aspect ratio only takes effect once it is made windowed.
         *        If the window is not resizable, this function does nothing.
         */
        DAEvoid SetAspectRatio(DAEint32 in_numerator, DAEint32 in_denominator) const noexcept;

        /**
         * \brief Sets the size, in screen coordinates, of the content area of the window.
         * \note  For full screen windows, this function updates the resolution of its desired video mode
         *        and switches to the video mode closest to it, without affecting the window's context.
         */
        DAEvoid SetSize(VkExtent2D const& in_size) const noexcept;

        /**
         * \brief Sets the opacity of the window, including any decorations.
         * \note  A window created with framebuffer transparency may not use whole window transparency.
         *        The results of doing this are undefined.
         */
        DAEvoid SetOpacity(DAEfloat in_opacity) const noexcept;

        /**
         * \brief Iconifies (minimizes) the window if it was previously restored.
         * \note  If the specified window is a full screen window,
         *        the original monitor resolution is restored until the window is restored.
         */
        DAEvoid Iconify() const noexcept;

        /**
         * \brief Restores the window if it was previously iconified (minimized) or maximized.
         * \note  If the specified window is a full screen window,
         *        the resolution chosen for the window is restored on the selected monitor.
         */
        DAEvoid Restore() const noexcept;

        /**
         * \brief Maximizes the window if it was previously not maximized.
         * \note  If the window is a full screen window, this function does nothing.
         */
        DAEvoid Maximize() const noexcept;

        /**
         * \brief Makes the window visible if it was previously hidden.
         * \note  By default, windowed mode windows are focused when shown.
         *        Set the 'focused_on_show' parameter to change this behavior for a newly created window.
         */
        DAEvoid Show() const noexcept;

        /**
         * \brief Hides the window if it was previously visible.
         */
        DAEvoid Hide() const noexcept;

        /**
         * \brief Brings the window to front and sets input focus. The window should already be visible and not iconified.
         * \note  By default, both windowed and full screen mode windows are focused when initially created.
         *        Also by default, windowed mode windows are focused when shown.
         */
        DAEvoid Focus() const noexcept;

        /**
         * \brief Requests user attention to the window.
         */
        DAEvoid RequestAttention() const noexcept;

        /**
         * \brief Makes the window full screen or windowed.
         * \note  When a window transitions from full screen to windowed mode, this function restores any previous window settings
         *        such as whether it is decorated, floating, resizable, has size or aspect ratio limits, etc.
         */
        DAEvoid SetFullscreen(DAEbool in_fullscreen) const noexcept;

        /**
         * \brief Sets whether the specified window is resizable by the user.
         */
        [[nodiscard]]
        DAEvoid SetResizable(DAEbool in_resizable) const noexcept;

        /**
         * \brief Sets whether the specified window has decorations.
         */
        [[nodiscard]]
        DAEvoid SetDecorated(DAEbool in_decorated) const noexcept;

        /**
         * \brief Sets whether the specified full screen window is iconified on focus loss.
         */
        [[nodiscard]]
        DAEvoid SetAutoIconified(DAEbool in_auto_iconified) const noexcept;

        /**
         * \brief Sets whether the specified window is floating, also called topmost or always-on-top..
         */
        [[nodiscard]]
        DAEvoid SetFloating(DAEbool in_floating) const noexcept;

        /**
         * \brief Sets whether the window will be given input focus when shown.
         */
        [[nodiscard]]
        DAEvoid SetFocusedOnShow(DAEbool in_focused_on_show) const noexcept;

        #pragma endregion

        #pragma region Getters

        /**
         * \return Opaque window object.
         */
        [[nodiscard]]
        GLFWwindow* GetHandle() const noexcept;

        /**
         * \return The UTF-8 encoded window title.
         */
        [[nodiscard]]
        std::string const& GetName() const noexcept;

        [[nodiscard]]
        DAEbool ShouldClose() const noexcept;

        /**
         * \return The window's current parameters.
         */
        [[nodiscard]]
        WindowParams GetParameters() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the content area of the window.
         */
        [[nodiscard]]
        VkOffset2D GetPosition() const noexcept;

        /**
         * \return The size, in screen coordinates, of the content area of the window.
         */
        [[nodiscard]]
        VkExtent2D GetSize() const noexcept;

        /**
         * \return The size, in pixels, of the framebuffer of the window.
         */
        [[nodiscard]]
        VkExtent2D GetFramebufferSize() const noexcept;

        /**
         * \return The size, in screen coordinates, of each edge of the frame of the window.
         * \note   This size includes the title bar, if the window has one.
         */
        [[nodiscard]]
        VkRect2D GetFrameSize() const noexcept;

        /**
         * \return The content scale for the window (the ratio between the current DPI and the platform's default DPI).
         * \note   On systems where each monitors can have its own content scale, the window content scale will depend
         *         on which monitor the system considers the window to be on.
         */
        [[nodiscard]]
        Vector2f GetContentScale() const noexcept;

        /**
         * \return The opacity of the window, including any decorations (between 0 and 1).
         * \note   If the system does not support whole window transparency, this function always returns one.
         */
        [[nodiscard]]
        DAEfloat GetOpacity() const noexcept;

        /**
         * \return True if the window is in full screen mode, else False.
         */
        [[nodiscard]]
        DAEbool IsFullscreen() const noexcept;

        /**
         * \return True if the window has input focus, else False.
         */
        [[nodiscard]]
        DAEbool IsFocused() const noexcept;

        /**
         * \return True if the window is iconified, else False.
         */
        [[nodiscard]]
        DAEbool IsIconified() const noexcept;

        /**
         * \return True if the window is maximized, else False.
         */
        [[nodiscard]]
        DAEbool IsMaximized() const noexcept;

        /**
         * \return True if the cursor is currently directly over the content area of the window, with no other windows between, else False.
         */
        [[nodiscard]]
        DAEbool IsHovered() const noexcept;

        /**
         * \return True if the window is visible, else False.
         */
        [[nodiscard]]
        DAEbool IsVisible() const noexcept;

        /**
         * \return True if the window is resizable by the user, else False.
         */
        [[nodiscard]]
        DAEbool IsResizable() const noexcept;

        /**
         * \return True if the window has decorations (such as a border, a close widget, etc.), else eFalse
         */
        [[nodiscard]]
        DAEbool IsDecorated() const noexcept;

        /**
         * \return True if the full screen window is iconified on focus loss, else False.
         */
        [[nodiscard]]
        DAEbool IsAutoIconified() const noexcept;

        /**
         * \return True if the window is floating (also called topmost or always-on-top), else False.
         */
        [[nodiscard]]
        DAEbool IsFloating() const noexcept;

        /**
         * \return True if the window has a transparent framebuffer, else False.
         */
        [[nodiscard]]
        DAEbool IsFramebufferTransparent() const noexcept;

        /**
         * \return True if the window will be given input focus when shown, else False.
         */
        [[nodiscard]]
        DAEbool IsFocusedOnShow() const noexcept;

        /**
         * \return True if the window is valid, else False.
         */
        [[nodiscard]]
        DAEbool IsValid() const noexcept;

        #pragma endregion

        #pragma region Operators

        Window& operator=(Window const& in_copy) = delete;
        Window& operator=(Window&&      in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE