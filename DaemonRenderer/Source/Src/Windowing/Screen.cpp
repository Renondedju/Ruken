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

#include "Windowing/Screen.hpp"

USING_DAEMON_NAMESPACE

Screen::Screen(GLFWmonitor* in_handle) :
    m_handle { in_handle },
    m_name   { glfwGetMonitorName(m_handle) }
{
    // Retrieves the size, in millimeters, of the display area of the monitor.
    glfwGetMonitorPhysicalSize(m_handle, &m_physical_size.width, &m_physical_size.height);

    // Retrieves the ratio between the current DPI and the platform's default DPI.
    glfwGetMonitorContentScale(m_handle, &m_content_scale.x, &m_content_scale.y);

    // Retrieves the position, in screen coordinates, of the upper-left corner of the monitor.
    glfwGetMonitorPos(m_handle, &m_position.x, &m_position.y);

    // Retrieves the work area defined as the area of the monitor not occluded by the operating system task bar where present.
    glfwGetMonitorWorkarea(m_handle, &m_work_area.position.x, &m_work_area.position.y, &m_work_area.extent.width, &m_work_area.extent.height);

    // Retrieves all video modes supported by the monitor.
    DAEint32 count;

    GLFWvidmode const* modes = glfwGetVideoModes(m_handle, &count);

    m_video_modes.reserve(count);

    for (DAEint32 i = 0; i < count; ++i)
    {
        m_video_modes.emplace_back(VideoMode {
            modes[i].width,
            modes[i].height,
            modes[i].redBits,
            modes[i].greenBits,
            modes[i].blueBits,
            modes[i].refreshRate
        });
    }
}

GLFWmonitor* Screen::GetHandle() const noexcept
{
    return m_handle;
}

String const& Screen::GetName() const noexcept
{
    return m_name;
}

Extent2D const& Screen::GetPhysicalSize() const noexcept
{
    return m_physical_size;
}

Scale2D const& Screen::GetContentScale() const noexcept
{
    return m_content_scale;
}

Position2D const& Screen::GetPosition() const noexcept
{
    return m_position;
}

Area2D const& Screen::GetWorkArea() const noexcept
{
    return m_work_area;
}

VideoMode const& Screen::GetCurrentVideoMode() const noexcept
{
    return *reinterpret_cast<VideoMode const*>(glfwGetVideoMode(m_handle));
}

Vector<VideoMode> const& Screen::GetVideoModes() const noexcept
{
    return m_video_modes;
}

GammaRamp const& Screen::GetGammaRamp() const noexcept
{
    return *reinterpret_cast<GammaRamp const*>(glfwGetGammaRamp(m_handle));
}

DAEvoid Screen::SetGamma(DAEfloat const in_gamma) const noexcept
{
    glfwSetGamma(m_handle, in_gamma);
}

DAEvoid Screen::SetGammaRamp(GammaRamp const& in_gamma_ramp) const noexcept
{
    glfwSetGammaRamp(m_handle, reinterpret_cast<GLFWgammaramp const*>(&in_gamma_ramp));
}