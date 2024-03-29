
#include "Windowing/Screen.hpp"

#ifdef RUKEN_OS_WINDOWS
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Screen::Screen(Logger* in_logger, GLFWmonitor* in_handle) noexcept:
    m_logger    {in_logger},
    m_handle    {in_handle},
    m_name      {glfwGetMonitorName(in_handle)}
{
    auto        count = 0;
    auto const* modes = glfwGetVideoModes(in_handle, &count);

    for (auto i = 0; i < count; ++i)
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

    glfwSetMonitorUserPointer(in_handle, this);
}

Screen::Screen(Screen&& in_move) noexcept:
    m_logger        {in_move.m_logger},
    m_handle        {in_move.m_handle},
    m_name          {std::move(in_move.m_name)},
    m_video_modes   {std::move(in_move.m_video_modes)}
{
    glfwSetMonitorUserPointer(m_handle, this);
}

#pragma endregion

#pragma region Methods

Screen* Screen::GetMonitorUserPointer(GLFWmonitor* in_monitor) noexcept
{
    return static_cast<Screen*>(glfwGetMonitorUserPointer(in_monitor));
}

GLFWmonitor* Screen::GetHandle() const noexcept
{
    return m_handle;
}

std::string const& Screen::GetName() const noexcept
{
    return m_name;
}

std::vector<VideoMode> const& Screen::GetVideoModes() const noexcept
{
    return m_video_modes;
}

Position2D Screen::GetPosition() const noexcept
{
    Position2D position = {}; 

    glfwGetMonitorPos(m_handle, &position.x, &position.y);

    return position;
}

Rect2D Screen::GetWorkArea() const noexcept
{
    Rect2D area = {};

    glfwGetMonitorWorkarea(m_handle, &area.position.x,
                                     &area.position.y,
                                     &area.extent.width,
                                     &area.extent.height);

    return area;
}

Extent2D Screen::GetPhysicalSize() const noexcept
{
    Extent2D extent = {};

    glfwGetMonitorPhysicalSize(m_handle, &extent.width, &extent.height);

    return extent;
}

Scale2D Screen::GetContentScale() const noexcept
{
    Scale2D scale = {};

    glfwGetMonitorContentScale(m_handle, &scale.x, &scale.y);

    return scale;
}

VideoMode Screen::GetCurrentVideoMode() const noexcept
{
    return *reinterpret_cast<VideoMode const*>(glfwGetVideoMode(m_handle));
}

GammaRamp Screen::GetGammaRamp() const noexcept
{
    return *reinterpret_cast<GammaRamp const*>(glfwGetGammaRamp(m_handle));
}

RkVoid Screen::SetGamma(RkFloat const in_gamma) const noexcept
{
    glfwSetGamma(m_handle, in_gamma);
}

RkVoid Screen::SetGammaRamp(GammaRamp const& in_gamma_ramp) const noexcept
{
    glfwSetGammaRamp(m_handle, reinterpret_cast<GLFWgammaramp const*>(&in_gamma_ramp));
}

#ifdef RUKEN_OS_WINDOWS

RkChar const* Screen::GetWin32Adapter() const noexcept
{
    return glfwGetWin32Adapter(m_handle);
}

RkChar const* Screen::GetWin32Monitor() const noexcept
{
    return glfwGetWin32Monitor(m_handle);
}

#endif

#pragma endregion

#pragma region Operators

Screen& Screen::operator=(Screen&& in_move) noexcept
{
    m_logger      = in_move.m_logger;
    m_handle      = in_move.m_handle;
    m_name        = std::move(in_move.m_name);
    m_video_modes = std::move(in_move.m_video_modes);

    glfwSetMonitorUserPointer(m_handle, this);

    return *this;
}

RkBool Screen::operator==(Screen const& in_other) const noexcept
{
    return m_handle == in_other.m_handle;
}

RkBool Screen::operator!=(Screen const& in_other) const noexcept
{
    return m_handle != in_other.m_handle;
}

#pragma endregion