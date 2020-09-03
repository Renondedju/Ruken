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

#include "Windowing/Window.hpp"

#ifdef RUKEN_OS_WINDOWS
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#undef CreateWindow

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Window::Window(Logger* in_logger, WindowParams const& in_params) noexcept:
    m_logger    {in_logger},
    m_name      {in_params.name}
{
    CreateWindow  (in_params);
    SetupCallbacks();
}

Window::Window(Window&& in_move) noexcept:
    m_logger                {in_move.m_logger},
    m_handle                {in_move.m_handle},
    m_name                  {std::move(in_move.m_name)},
    on_closed               {std::move(in_move.on_closed)},
    on_refreshed            {std::move(in_move.on_refreshed)},
    on_moved                {std::move(in_move.on_moved)},
    on_resized              {std::move(in_move.on_resized)},
    on_focused              {std::move(in_move.on_focused)},
    on_iconified            {std::move(in_move.on_iconified)},
    on_maximized            {std::move(in_move.on_maximized)},
    on_framebuffer_resized  {std::move(in_move.on_framebuffer_resized)},
    on_content_rescaled     {std::move(in_move.on_content_rescaled)}
{
    in_move.m_handle = nullptr;

    glfwSetWindowUserPointer(m_handle, this);
}

Window::~Window() noexcept
{
    if (!m_handle)
        return;

    glfwDestroyWindow(m_handle);
}

#pragma endregion

#pragma region Methods

#pragma region Callbacks

DAEvoid Window::WindowPosCallback(GLFWwindow* in_window, DAEint32 const in_x_pos, DAEint32 const in_y_pos) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_moved.Invoke(in_x_pos, in_y_pos);
}

DAEvoid Window::WindowSizeCallback(GLFWwindow* in_window, DAEint32 const in_width, DAEint32 const in_height) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_resized.Invoke(in_width, in_height);
}

DAEvoid Window::WindowCloseCallback(GLFWwindow* in_window) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_closed.Invoke();
}

DAEvoid Window::WindowRefreshCallback(GLFWwindow* in_window) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_refreshed.Invoke();
}

DAEvoid Window::WindowFocusCallback(GLFWwindow* in_window, DAEint32 const in_focused) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_focused.Invoke(in_focused);
}

DAEvoid Window::WindowIconifyCallback(GLFWwindow* in_window, DAEint32 const in_iconified) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_iconified.Invoke(in_iconified);
}

DAEvoid Window::WindowMaximizeCallback(GLFWwindow* in_window, DAEint32 const in_maximized) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_maximized.Invoke(in_maximized);
}

DAEvoid Window::FramebufferSizeCallback(GLFWwindow* in_window, DAEint32 const in_width, DAEint32 const in_height) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_framebuffer_resized.Invoke(in_width, in_height);
}

DAEvoid Window::WindowContentScaleCallback(GLFWwindow* in_window, DAEfloat const in_x_scale, DAEfloat const in_y_scale) noexcept
{
    if (auto* window = GetWindowUserPointer(in_window))
        window->on_content_rescaled.Invoke(in_x_scale, in_y_scale);
}

#pragma endregion

Window* Window::GetWindowUserPointer(GLFWwindow* in_window) noexcept
{
    return static_cast<Window*>(glfwGetWindowUserPointer(in_window));
}

DAEvoid Window::CreateWindow(WindowParams const& in_params) noexcept
{
    glfwDefaultWindowHints();

    // This is needed because GLFW defaults to en OpenGL context otherwise.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Setups the window creation's hints.
    glfwWindowHint(GLFW_RESIZABLE,               in_params.resizable);
    glfwWindowHint(GLFW_VISIBLE,                 in_params.visible);
    glfwWindowHint(GLFW_DECORATED,               in_params.decorated);
    glfwWindowHint(GLFW_FOCUSED,                 in_params.focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY,            in_params.auto_iconified);
    glfwWindowHint(GLFW_FLOATING,                in_params.floating);
    glfwWindowHint(GLFW_MAXIMIZED,               in_params.maximized);
    glfwWindowHint(GLFW_CENTER_CURSOR,           in_params.cursor_centered);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, in_params.transparent_framebuffer);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW,           in_params.focused_on_show);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR,        in_params.scale_to_monitor);

    if (in_params.fullscreen)
    {
        auto* const        monitor    = glfwGetPrimaryMonitor();
        auto  const* const video_mode = glfwGetVideoMode     (monitor);

        m_handle = glfwCreateWindow(video_mode->width, video_mode->height, m_name.c_str(), monitor, nullptr);
    }

    else
        m_handle = glfwCreateWindow(in_params.size.width, in_params.size.height, m_name.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetWindowOpacity    (m_handle, in_params.opacity);
}

DAEvoid Window::SetupCallbacks() noexcept
{
    glfwSetWindowPosCallback         (m_handle, &WindowPosCallback);
    glfwSetWindowSizeCallback        (m_handle, &WindowSizeCallback);
    glfwSetWindowCloseCallback       (m_handle, &WindowCloseCallback);
    glfwSetWindowRefreshCallback     (m_handle, &WindowRefreshCallback);
    glfwSetWindowFocusCallback       (m_handle, &WindowFocusCallback);
    glfwSetWindowIconifyCallback     (m_handle, &WindowIconifyCallback);
    glfwSetWindowMaximizeCallback    (m_handle, &WindowMaximizeCallback);
    glfwSetFramebufferSizeCallback   (m_handle, &FramebufferSizeCallback);
    glfwSetWindowContentScaleCallback(m_handle, &WindowContentScaleCallback);
}

#pragma region Setters

DAEvoid Window::SetName(DAEchar const* in_name) noexcept
{
    m_name = in_name;

    glfwSetWindowTitle(m_handle, in_name);
}

DAEvoid Window::SetPosition(Position2D const& in_position) const noexcept
{
    glfwSetWindowPos(m_handle, in_position.x, in_position.y);
}

DAEvoid Window::SetSizeLimits(Extent2D const& in_min_size, Extent2D const& in_max_size) const noexcept
{
    glfwSetWindowSizeLimits(m_handle, in_min_size.width, in_min_size.height,
                                      in_max_size.width, in_max_size.height);
}

DAEvoid Window::SetAspectRatio(DAEint32 const in_numerator, DAEint32 const in_denominator) const noexcept
{
    glfwSetWindowAspectRatio(m_handle, in_numerator, in_denominator);
}

DAEvoid Window::SetSize(Extent2D const& in_size) const noexcept
{
    glfwSetWindowSize(m_handle, in_size.width, in_size.height);
}

DAEvoid Window::SetOpacity(DAEfloat const in_opacity) const noexcept
{
    glfwSetWindowOpacity(m_handle, in_opacity);
}

DAEvoid Window::Iconify() const noexcept
{
    glfwIconifyWindow(m_handle);
}

DAEvoid Window::Restore() const noexcept
{
    glfwRestoreWindow(m_handle);
}

DAEvoid Window::Maximize() const noexcept
{
    glfwMaximizeWindow(m_handle);
}

DAEvoid Window::Show() const noexcept
{
    glfwShowWindow(m_handle);
}

DAEvoid Window::Hide() const noexcept
{
    glfwHideWindow(m_handle);
}

DAEvoid Window::Focus() const noexcept
{
    glfwFocusWindow(m_handle);
}

DAEvoid Window::RequestAttention() const noexcept
{
    glfwRequestWindowAttention(m_handle);
}

DAEvoid Window::SetFullscreen(DAEbool const in_fullscreen) const noexcept
{
    auto* const  monitor    = glfwGetPrimaryMonitor();
    auto  const* video_mode = glfwGetVideoMode     (monitor);

    if (in_fullscreen)
        glfwSetWindowMonitor(m_handle, monitor, 0, 0, video_mode->width, video_mode->height, GLFW_DONT_CARE);
    else
        glfwSetWindowMonitor(m_handle, nullptr, 0, 0, video_mode->width, video_mode->height, GLFW_DONT_CARE);
}

DAEvoid Window::SetDecorated(DAEbool const in_decorated) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_DECORATED, in_decorated);
}

DAEvoid Window::SetResizable(DAEbool const in_resizable) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_RESIZABLE, in_resizable);
}

DAEvoid Window::SetFloating(DAEbool const in_floating) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_FLOATING, in_floating);
}

DAEvoid Window::SetAutoIconified(DAEbool const in_auto_iconified) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_AUTO_ICONIFY, in_auto_iconified);
}

DAEvoid Window::SetFocusedOnShow(DAEbool const in_focused_on_show) const noexcept
{
    glfwSetWindowAttrib(m_handle, GLFW_FOCUS_ON_SHOW, in_focused_on_show);
}

#pragma endregion

#pragma region Getters

GLFWwindow* Window::GetHandle() const noexcept
{
    return m_handle;
}

std::string const& Window::GetName() const noexcept
{
    return m_name;
}

DAEbool Window::ShouldClose() const noexcept
{
    return glfwWindowShouldClose(m_handle) == GLFW_TRUE;
}

Position2D Window::GetPosition() const noexcept
{
    Position2D position = {};

    glfwGetWindowPos(m_handle, &position.x, &position.y);

    return position;
}

Extent2D Window::GetSize() const noexcept
{
    Extent2D size = {};

    glfwGetWindowSize(m_handle, &size.width, &size.height);

    return size;
}

Extent2D Window::GetFramebufferSize() const noexcept
{
    Extent2D size = {};

    glfwGetFramebufferSize(m_handle, &size.width, &size.height);

    return size;
}

Rect2D Window::GetFrameSize() const noexcept
{
    Rect2D frame = {};

    glfwGetWindowFrameSize(m_handle, 
                           &frame.position.x,
                           &frame.position.y,
                           &frame.extent.width,
                           &frame.extent.height);

    return frame;
}

Scale2D Window::GetContentScale() const noexcept
{
    Scale2D scale = {};

    glfwGetWindowContentScale(m_handle, &scale.x, &scale.y);

    return scale;
}

DAEfloat Window::GetOpacity() const noexcept
{
    return glfwGetWindowOpacity(m_handle);
}

DAEbool Window::IsFullscreen() const noexcept
{
    return glfwGetWindowMonitor(m_handle) != nullptr;
}

DAEbool Window::IsFocused() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FOCUSED);
}

DAEbool Window::IsIconified() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_ICONIFIED);
}

DAEbool Window::IsMaximized() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_MAXIMIZED);
}

DAEbool Window::IsHovered() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_HOVERED);
}

DAEbool Window::IsVisible() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_VISIBLE);
}

DAEbool Window::IsResizable() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_RESIZABLE);
}

DAEbool Window::IsDecorated() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_DECORATED);
}

DAEbool Window::IsAutoIconified() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_AUTO_ICONIFY);
}

DAEbool Window::IsFloating() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FLOATING);
}

DAEbool Window::IsFramebufferTransparent() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_TRANSPARENT_FRAMEBUFFER);
}

DAEbool Window::IsFocusedOnShow() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FOCUS_ON_SHOW);
}

DAEbool Window::IsValid() const noexcept
{
    return m_handle != nullptr;
}

#ifdef RUKEN_OS_WINDOWS

HWND Window::GetWin32Window() const noexcept
{
    return glfwGetWin32Window(m_handle);
}

#endif

#pragma endregion

#pragma endregion

#pragma region Operators

Window& Window::operator=(Window&& in_move) noexcept
{
    m_logger               = in_move.m_logger;
    m_handle               = in_move.m_handle;
    m_name                 = std::move(in_move.m_name);
    on_closed              = std::move(in_move.on_closed);
    on_refreshed           = std::move(in_move.on_refreshed);
    on_moved               = std::move(in_move.on_moved);
    on_resized             = std::move(in_move.on_resized);
    on_focused             = std::move(in_move.on_focused);
    on_iconified           = std::move(in_move.on_iconified);
    on_maximized           = std::move(in_move.on_maximized);
    on_framebuffer_resized = std::move(in_move.on_framebuffer_resized);
    on_content_rescaled    = std::move(in_move.on_content_rescaled);

    in_move.m_handle = nullptr;

    glfwSetWindowUserPointer(m_handle, this);

    return *this;
}

DAEbool Window::operator==(Window const& in_other) const noexcept
{
    return m_handle == in_other.m_handle;
}

DAEbool Window::operator!=(Window const& in_other) const noexcept
{
    return m_handle != in_other.m_handle;
}

#pragma endregion