/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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
 * \brief This class manages a GLFW window.
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
        String      m_name;

        #pragma endregion

        #pragma region Default Constructor

        explicit Window(WindowParameters&& in_parameters) noexcept;

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
         * \brief This is called when the content area of the window needs to be redrawn.
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

        #pragma region Members

        Event<Position2D>   on_moved;
        Event<Extent2D>     on_resized;
        Event<>             on_closed;
        Event<>             on_refreshed;
        Event<DAEbool>      on_focused;
        Event<DAEbool>      on_iconified;
        Event<DAEbool>      on_maximized;
        Event<Extent2D>     on_framebuffer_resized;
        Event<Scale2D>      on_content_rescaled;

        #pragma endregion

        #pragma region Constructors and Destructor

        Window() = delete;

        Window(Window const&    in_other) noexcept = delete;
        Window(Window&&         in_other) noexcept = default;

        ~Window() noexcept;

        #pragma endregion

        #pragma region Operators

        Window& operator=(Window const& in_other) noexcept = delete;
        Window& operator=(Window&&      in_other) noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief
         *
         * \param in_name 
         */
        DAEvoid SetName(String&& in_name) noexcept;

        /**
         * \brief
         *
         * \param in_position 
         */
        DAEvoid SetPosition(Position2D&& in_position) const noexcept;

        /**
         * \brief
         *
         * \param in_min 
         * \param in_max 
         */
        DAEvoid SetSizeLimits(Extent2D&& in_min, Extent2D&& in_max) const noexcept;

        /**
         * \brief
         *
         * \param in_numerator 
         * \param in_denominator 
         */
        DAEvoid SetAspectRatio(DAEint32 in_numerator, DAEint32 in_denominator) const noexcept;

        /**
         * \brief
         *
         * \param in_size 
         */
        DAEvoid SetSize(Extent2D&& in_size) const noexcept;

        /**
         * \brief
         *
         * \param in_opacity 
         */
        DAEvoid SetOpacity(DAEfloat in_opacity) const noexcept;

        /**
         * \brief
         *
         * \param in_fullscreen 
         */
        DAEvoid SetFullscreen(DAEbool in_fullscreen) const noexcept;

        /**
         * \brief 
         */
        DAEvoid Iconify() const noexcept;

        /**
         * \brief 
         */
        DAEvoid Restore() const noexcept;

        /**
         * \brief 
         */
        DAEvoid Maximize() const noexcept;

        /**
         * \brief 
         */
        DAEvoid Show() const noexcept;

        /**
         * \brief 
         */
        DAEvoid Hide() const noexcept;

        /**
         * \brief 
         */
        DAEvoid Focus() const noexcept;

        /**
         * \brief 
         */
        DAEvoid RequestAttention() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] GLFWwindow* GetHandle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] String const& GetName() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Position2D GetPosition() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Extent2D GetSize() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Extent2D GetFramebufferSize() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Scale2D GetContentScale() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEfloat GetOpacity() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsFullscreen() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsFocused() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsIconified() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsMaximized() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsHovered() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsVisible() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsResizable() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsDecorated() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsAutoIconified() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsFloating() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsFramebufferTransparent() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsFocusedOnShow() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] DAEbool IsValid() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE