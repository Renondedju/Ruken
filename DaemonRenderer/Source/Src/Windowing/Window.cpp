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

#include "Windowing/WindowManager.hpp"

#include "Rendering/Renderer.hpp"

#include "Vulkan/Instance.hpp"
#include "Vulkan/PhysicalDevice.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Window::Window() noexcept :
    m_handle    { nullptr },
    m_surface   { nullptr }
{

}

Window::~Window() noexcept
{
    vkDestroySurfaceKHR(GRenderer->GetInstance()->GetHandle(), m_surface, nullptr);

    glfwDestroyWindow(m_handle);
}

#pragma endregion

#pragma region Methods

#pragma region Callbacks

DAEvoid Window::WindowPosCallback(GLFWwindow* in_window, DAEint32 const in_x_pos, DAEint32 const in_y_pos) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_moved.Invoke({ in_x_pos, in_y_pos });
}

DAEvoid Window::WindowSizeCallback(GLFWwindow* in_window, DAEint32 const in_width, DAEint32 const in_height) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_resized.Invoke({ in_width, in_height });
}

DAEvoid Window::WindowCloseCallback(GLFWwindow* in_window) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_closed.Invoke();
}

DAEvoid Window::WindowRefreshCallback(GLFWwindow* in_window) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_refreshed.Invoke();
}

DAEvoid Window::WindowFocusCallback(GLFWwindow* in_window, DAEint32 const in_focused) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_focused.Invoke(in_focused);
}

DAEvoid Window::WindowIconifyCallback(GLFWwindow* in_window, DAEint32 const in_iconified) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_iconified.Invoke(in_iconified);
}

DAEvoid Window::WindowMaximizeCallback(GLFWwindow* in_window, DAEint32 const in_maximized) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_maximized.Invoke(in_maximized);
}

DAEvoid Window::FramebufferSizeCallback(GLFWwindow* in_window, DAEint32 const in_width, DAEint32 const in_height) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_framebuffer_resized.Invoke({ in_width, in_height });
}

DAEvoid Window::WindowContentScaleCallback(GLFWwindow* in_window, DAEfloat const in_x_scale, DAEfloat const in_y_scale) noexcept
{
    if (Window* window = GetWindowUserPointer(in_window))
        window->on_content_rescaled.Invoke({ in_x_scale, in_y_scale });
}

#pragma endregion

Window* Window::GetWindowUserPointer(GLFWwindow* in_window) noexcept
{
    return static_cast<Window*>(glfwGetWindowUserPointer(in_window));
}

DAEvoid Window::SetupWindow(WindowParams&& in_parameters) noexcept
{
    // 
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // 
    glfwWindowHint(GLFW_RESIZABLE,               in_parameters.resizable);
    glfwWindowHint(GLFW_VISIBLE,                 in_parameters.visible);
    glfwWindowHint(GLFW_DECORATED,               in_parameters.decorated);
    glfwWindowHint(GLFW_FOCUSED,                 in_parameters.focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY,            in_parameters.auto_iconified);
    glfwWindowHint(GLFW_FLOATING,                in_parameters.floating);
    glfwWindowHint(GLFW_MAXIMIZED,               in_parameters.maximized);
    glfwWindowHint(GLFW_CENTER_CURSOR,           in_parameters.cursor_centered);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, in_parameters.transparent_framebuffer);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW,           in_parameters.focused_on_show);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR,        in_parameters.scale_to_monitor);

    if (in_parameters.fullscreen)
    {
        GLFWmonitor*       monitor    = glfwGetPrimaryMonitor();
        GLFWvidmode const* video_mode = glfwGetVideoMode     (monitor);

        m_handle = glfwCreateWindow(video_mode->width, video_mode->height, m_name.c_str(), monitor, nullptr);
    }

    else
        m_handle = glfwCreateWindow(in_parameters.size.width, in_parameters.size.height, m_name.c_str(), nullptr, nullptr);

    glfwSetWindowOpacity(m_handle, in_parameters.opacity);
}

DAEvoid Window::SetupCallbacks() const noexcept
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

DAEvoid Window::Initialize(WindowParams&& in_parameters) noexcept
{
    m_name = std::move(in_parameters.name);

    SetupWindow(std::move(in_parameters));

    glfwSetWindowUserPointer(m_handle, this);

    SetupCallbacks();

    if (glfwCreateWindowSurface(GRenderer->GetInstance()->GetHandle(), m_handle, nullptr, &m_surface) == VK_SUCCESS)
    {
        // TODO : Create RenderContext here.
    }
}

DAEvoid Window::SetClosed(DAEbool const in_should_close) const noexcept
{
    glfwSetWindowShouldClose(m_handle, in_should_close);
}

DAEvoid Window::SetName(std::string&& in_name) noexcept
{
    m_name = std::move(in_name);

    glfwSetWindowTitle(m_handle, m_name.c_str());
}

DAEvoid Window::SetPosition(VkOffset2D&& in_position) const noexcept
{
    glfwSetWindowPos(m_handle, in_position.x, in_position.y);
}

DAEvoid Window::SetSizeLimits(VkExtent2D&& in_min, VkExtent2D&& in_max) const noexcept
{
    glfwSetWindowSizeLimits(m_handle, in_min.width, in_min.height, in_max.width, in_max.height);
}

DAEvoid Window::SetAspectRatio(DAEint32 const in_numerator, DAEint32 const in_denominator) const noexcept
{
    glfwSetWindowAspectRatio(m_handle, in_numerator, in_denominator);
}

DAEvoid Window::SetSize(VkExtent2D&& in_size) const noexcept
{
    glfwSetWindowSize(m_handle, in_size.width, in_size.height);
}

DAEvoid Window::SetOpacity(DAEfloat const in_opacity) const noexcept
{
    glfwSetWindowOpacity(m_handle, in_opacity);
}

DAEvoid Window::SetFullscreen(DAEbool const in_fullscreen) const noexcept
{
    if (in_fullscreen)
    {
        
    }

    else
    {
        glfwSetWindowMonitor(m_handle, nullptr, 0, 0, 1600, 900, GLFW_DONT_CARE);
    }
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

DAEbool Window::ShouldClose() const noexcept
{
    return glfwWindowShouldClose(m_handle);
}

GLFWwindow* Window::GetHandle() const noexcept
{
    return m_handle;
}

VkSurfaceKHR Window::GetSurface() const noexcept
{
    return m_surface;
}

std::string const& Window::GetName() const noexcept
{
    return m_name;
}

WindowParams Window::GetParameters() const noexcept
{
    return
    {
        m_name,
        GetPosition(),
        GetSize(),
        GetOpacity(),
        IsFullscreen(),
        IsFocused(),
        IsIconified(),
        IsMaximized(),
        IsVisible(),
        IsResizable(),
        IsDecorated(),
        IsAutoIconified(),
        IsFloating(),
        IsFramebufferTransparent(),
        IsFocusedOnShow()
    };
}

VkOffset2D Window::GetPosition() const noexcept
{
    VkOffset2D position;

    glfwGetWindowPos(m_handle, &position.x, &position.y);

    return position;
}

VkExtent2D Window::GetSize() const noexcept
{
    VkExtent2D size;

    glfwGetWindowSize(m_handle, reinterpret_cast<DAEint*>(&size.width), reinterpret_cast<DAEint*>(&size.height));

    return size;
}

VkExtent2D Window::GetFramebufferSize() const noexcept
{
    VkExtent2D size;

    glfwGetFramebufferSize(m_handle, reinterpret_cast<DAEint*>(&size.width), reinterpret_cast<DAEint*>(&size.height));

    return size;
}

VkRect2D Window::GetFrameSize() const noexcept
{
    VkRect2D frame;

    glfwGetWindowFrameSize(m_handle, &frame.offset.x, &frame.offset.y, reinterpret_cast<DAEint*>(&frame.extent.width), reinterpret_cast<DAEint*>(&frame.extent.height));

    return frame;
}

Vector2f Window::GetContentScale() const noexcept
{
    Vector2f scale;

    glfwGetWindowContentScale(m_handle, &scale.x, &scale.y);

    return scale;
}

DAEfloat Window::GetOpacity() const noexcept
{
    return glfwGetWindowOpacity(m_handle);
}

DAEbool Window::IsFullscreen() const noexcept
{
    return glfwGetWindowMonitor(m_handle);
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
    return glfwGetWindowAttrib(m_handle, GLFW_AUTO_ICONIFY );
}

DAEbool Window::IsFloating() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FLOATING );
}

DAEbool Window::IsFramebufferTransparent() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_TRANSPARENT_FRAMEBUFFER );
}

DAEbool Window::IsFocusedOnShow() const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FOCUS_ON_SHOW );
}

DAEbool Window::IsValid() const noexcept
{
    return m_handle;
}