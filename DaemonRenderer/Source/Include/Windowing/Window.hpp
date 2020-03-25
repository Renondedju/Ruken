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

#include "Utility.hpp"

#include "Functional/Event.hpp"

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

        #pragma region Constructor

        explicit Window() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief This is called when the window is moved.
         *
         * \param in_window The window that was moved.
         * \param in_x_pos  The new x-coordinate, in screen coordinates, of the upper-left corner of the content area of the window.
         * \param in_y_pos  The new y-coordinate, in screen coordinates, of the upper-left corner of the content area of the window.
         */
        static DAEvoid WindowPosCallback(GLFWwindow* in_window, DAEint32 in_x_pos, DAEint32 in_y_pos) noexcept;

        /**
         * \brief This is called when the window is resized.
         *
         * \param in_window The window that was resized.
         * \param in_width  The new width, in screen coordinates, of the window.
         * \param in_height The new height, in screen coordinates, of the window.
         */
        static DAEvoid WindowSizeCallback(GLFWwindow* in_window, DAEint32 in_width, DAEint32 in_height) noexcept;

        /**
         * \brief This is called when the user attempts to close the window, for example by clicking the close widget in the title bar.
         *
         * \param in_window The window that the user attempted to close.
         */
        static DAEvoid WindowCloseCallback(GLFWwindow* in_window) noexcept;

        /**
         * \brief This is called when the content area of the window needs to be redrawn.
         *
         * \param in_window The window whose content needs to be refreshed.
         */
        static DAEvoid WindowRefreshCallback(GLFWwindow* in_window) noexcept;

        /**
         * \brief This is called when the window gains or loses input focus.
         *
         * \param in_window  The window that gained or lost input focus.
         * \param in_focused 'GLFW_TRUE' if the window was given input focus, or 'GLFW_FALSE' if it lost it.
         */
        static DAEvoid WindowFocusCallback(GLFWwindow* in_window, DAEint32 in_focused) noexcept;

        /**
         * \brief This is called when the window is iconified or restored.
         *
         * \param in_window    The window that was iconified or restored.
         * \param in_iconified 'GLFW_TRUE' if the window was iconified, or 'GLFW_FALSE' if it was restored.
         */
        static DAEvoid WindowIconifyCallback(GLFWwindow* in_window, DAEint32 in_iconified) noexcept;

        /**
         * \brief This is called when the window is maximized or restored.
         *
         * \param in_window    The window that was maximized or restored.
         * \param in_maximized 'GLFW_TRUE' if the window was maximized, or 'GLFW_FALSE' if it was restored.
         */
        static DAEvoid WindowMaximizeCallback(GLFWwindow* in_window, DAEint32 in_maximized) noexcept;

        /**
         * \brief This is called when the framebuffer of the specified window is resized.
         *
         * \param in_window The window whose framebuffer was resized.
         * \param in_width  The new width, in pixels, of the framebuffer.
         * \param in_height The new height, in pixels, of the framebuffer.
         */
        static DAEvoid FramebufferSizeCallback(GLFWwindow* in_window, DAEint32 in_width, DAEint32 in_height) noexcept;

        /**
         * \brief This is called when the content scale of the specified window changes.
         *
         * \param in_window  The window whose content scale changed.
         * \param in_x_scale The new x-axis content scale of the window.
         * \param in_y_scale The new y-axis content scale of the window.
         */
        static DAEvoid WindowContentScaleCallback(GLFWwindow* in_window, DAEfloat in_x_scale, DAEfloat in_y_scale) noexcept;

        /**
         * \param in_window The window whose pointer to return.
         *
         * \return The current value of the user-defined pointer of the specified window.
         */
        static Window* GetWindowUserPointer(GLFWwindow* in_window) noexcept;

        /**
         * \brief Creates a new window with the specified parameters.
         *
         * \param in_parameters The parameters to use.
         */
        DAEvoid SetupWindow(WindowParameters&& in_parameters) noexcept;

        /**
         * \brief Sets the callbacks for the newly created window.
         */
        DAEvoid SetupCallbacks() const noexcept;

        #pragma endregion

    public:

        #pragma region Events

        /** \brief Called when the window is moved. */
        Event<Position2D> on_moved;

        /** \brief Called when the window is resized. */
        Event<Extent2D> on_resized;

        /** \brief Called when the user attempts to close the window. */
        Event<> on_closed;

        /** \brief Called when the content area of the window needs to be redrawn. */
        Event<> on_refreshed;

        /** \brief Called when the window gains or loses input focus. */
        Event<DAEbool> on_focused;

        /** \brief Called when the window is iconified or restored. */
        Event<DAEbool> on_iconified;

        /** \brief Called when the window is maximized or restored. */
        Event<DAEbool> on_maximized;

        /** \brief Called when the framebuffer of the specified window is resized. */
        Event<Extent2D> on_framebuffer_resized;

        /** \brief Called when the content scale of the specified window changes. */
        Event<Scale2D> on_content_rescaled;

        #pragma endregion

        #pragma region Constructors

        Window(Window const&    in_other) = delete;
        Window(Window&&         in_other) = default;
        ~Window() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Initializes the windows
         */
        DAEvoid Initialize(WindowParameters&& in_parameters) noexcept;

        /**
         * \return The value of the close flag of the window.
         */
        [[nodiscard]]
        DAEbool ShouldClose() const noexcept;

        /**
         * \brief This can be used to override the user's attempt to close this window,
         *        or to signal that it should be closed.
         * \param in_should_close The new value.
         */
        DAEvoid SetClosed(DAEbool in_should_close) const noexcept;

        /**
         * \brief Sets the title, encoded as UTF-8, of this window.
         * \param in_name The UTF-8 encoded window title.
         */
        DAEvoid SetName(std::string&& in_name) noexcept;

        /**
         * \brief Sets the position, in screen coordinates, of the upper-left corner of the content area of the window.
         * \param in_position The coordinates of the upper-left corner of the content area.
         * \note If the window is a full screen window, this function does nothing.
         */
        DAEvoid SetPosition(Position2D&& in_position) const noexcept;

        /**
         * \brief Sets the size limits of the content area of the window.
         *
         * \param in_min The minimum size, in screen coordinates, of the content area.
         * \param in_max The maximum size, in screen coordinates, of the content area.
         *
         * \note If the window is full screen, the size limits only take effect once it is made windowed.
         *       If the window is not resizable, this function does nothing.
         */
        DAEvoid SetSizeLimits(Extent2D&& in_min, Extent2D&& in_max) const noexcept;

        /**
         * \brief Sets the required aspect ratio of the content area of the window.
         *
         * \param in_numerator   The numerator of the desired aspect ratio.
         * \param in_denominator The denominator of the desired aspect ratio.
         *
         * \note If the window is full screen, the aspect ratio only takes effect once it is made windowed.
         *       If the window is not resizable, this function does nothing.
         */
        DAEvoid SetAspectRatio(DAEint32 in_numerator, DAEint32 in_denominator) const noexcept;

        /**
         * \brief Sets the size, in screen coordinates, of the content area of the window.
         * \param in_size The desired size, in screen coordinates, of the window content area.
         * \note For full screen windows, this function updates the resolution of its desired video mode
         *       and switches to the video mode closest to it, without affecting the window's context.
         */
        DAEvoid SetSize(Extent2D&& in_size) const noexcept;

        /**
         * \brief Sets the opacity of the window, including any decorations.
         * \param in_opacity The desired opacity of the window.
         *        Must be between 0 and 1, were 0 is transparent and 1 is opaque.
         * \note A window created with framebuffer transparency may not use whole window transparency.
         *       The results of doing this are undefined.
         */
        DAEvoid SetOpacity(DAEfloat in_opacity) const noexcept;

        /**
         * \brief Iconifies (minimizes) the window if it was previously restored.
         *        If the window is already iconified, this function does nothing.
         * \note If the specified window is a full screen window,
         *       the original monitor resolution is restored until the window is restored.
         */
        DAEvoid Iconify() const noexcept;

        /**
         * \brief Restores the window if it was previously iconified (minimized) or maximized.
         *        If the window is already restored, this function does nothing.
         * \note If the specified window is a full screen window,
         *       the resolution chosen for the window is restored on the selected monitor.
         */
        DAEvoid Restore() const noexcept;

        /**
         * \brief Maximizes the window if it was previously not maximized.
         *        If the window is already maximized, this function does nothing.
         * \note If the window is a full screen window, this function does nothing.
         */
        DAEvoid Maximize() const noexcept;

        /**
         * \brief Makes the window visible if it was previously hidden.
         *        If the window is already visible or is in full screen mode, this function does nothing.
         * \note By default, windowed mode windows are focused when shown.
         *       Set the 'focused_on_show' parameter to change this behavior for a newly created window.
         */
        DAEvoid Show() const noexcept;

        /**
         * \brief Hides the window if it was previously visible.
         *        If the window is already hidden or is in full screen mode, this function does nothing.
         */
        DAEvoid Hide() const noexcept;

        /**
         * \brief Brings the window to front and sets input focus. The window should already be visible and not iconified.
         * \note By default, both windowed and full screen mode windows are focused when initially created.
         *       Also by default, windowed mode windows are focused when shown.
         */
        DAEvoid Focus() const noexcept;

        /**
         * \brief Requests user attention to the window.
         * \note On platforms where this is not supported, attention is requested to the application as a whole.
         */
        DAEvoid RequestAttention() const noexcept;

        /**
         * \brief Makes the window full screen or windowed.
         * \param in_fullscreen The new mode of the window.
         * \note When a window transitions from full screen to windowed mode, this function restores any previous window settings
         *       such as whether it is decorated, floating, resizable, has size or aspect ratio limits, etc.
         */
        DAEvoid SetFullscreen(DAEbool in_fullscreen) const noexcept;

        // --- Getters

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

        /**
         * \return The window's current parameters.
         */
        [[nodiscard]]
        WindowParameters GetParameters() const noexcept;

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
         * \note This size includes the title bar, if the window has one.
         */
        [[nodiscard]]
        Square2D GetFrameSize() const noexcept;

        /**
         * \brief The content scale is the ratio between the current DPI and the platform's default DPI.
         * \return The content scale for the window.
         * \note On systems where each monitors can have its own content scale,
         *       the window content scale will depend on which monitor the system considers the window to be on.
         */
        [[nodiscard]]
        Scale2D GetContentScale() const noexcept;

        /**
         * \brief The opacity (or alpha) value is a positive finite number between zero and one,
         *        where zero is fully transparent and one is fully opaque.
         * \return The opacity of the window, including any decorations.
         * \note If the system does not support whole window transparency, this function always returns one.
         */
        [[nodiscard]]
        DAEfloat GetOpacity() const noexcept;

        /**
         * \return Whether the window is in full screen mode.
         */
        [[nodiscard]]
        DAEbool IsFullscreen() const noexcept;

        /**
         * \return Whether the window has input focus.
         */
        [[nodiscard]]
        DAEbool IsFocused() const noexcept;

        /**
         * \return Whether the window is iconified.
         */
        [[nodiscard]]
        DAEbool IsIconified() const noexcept;

        /**
         * \return Whether the window is maximized.
         */
        [[nodiscard]]
        DAEbool IsMaximized() const noexcept;

        /**
         * \return Whether the cursor is currently directly over the content area of the window, with no other windows between.
         */
        [[nodiscard]]
        DAEbool IsHovered() const noexcept;

        /**
         * \return Whether the window is visible.
         */
        [[nodiscard]]
        DAEbool IsVisible() const noexcept;

        /**
         * \return Whether the window is resizable by the user.
         */
        [[nodiscard]]
        DAEbool IsResizable() const noexcept;

        /**
         * \return Whether the window has decorations such as a border, a close widget, etc.
         */
        [[nodiscard]]
        DAEbool IsDecorated() const noexcept;

        /**
         * \return Whether the full screen window is iconified on focus loss, a close widget, etc.
         */
        [[nodiscard]]
        DAEbool IsAutoIconified() const noexcept;

        /**
         * \return Whether the window is floating, also called topmost or always-on-top.
         */
        [[nodiscard]]
        DAEbool IsFloating() const noexcept;

        /**
         * \return Whether the window has a transparent framebuffer, i.e. the window contents is composited with the background using the window framebuffer alpha channel.
         */
        [[nodiscard]]
        DAEbool IsFramebufferTransparent() const noexcept;

        /**
         * \return Whether the window will be given input focus when shown.
         */
        [[nodiscard]]
        DAEbool IsFocusedOnShow() const noexcept;

        /**
         * \return Whether the window is valid.
         */
        [[nodiscard]]
        DAEbool IsValid() const noexcept;

        #pragma endregion

        #pragma region Operators

        Window& operator=(Window const& in_other) = delete;
        Window& operator=(Window&&      in_other) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE