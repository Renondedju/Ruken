
#include "Windowing/Screen.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Screen::Screen(Logger* in_logger, GLFWmonitor* in_handle) noexcept:
    m_logger {in_logger},
    m_handle {in_handle},
    m_name   {glfwGetMonitorName(in_handle)}
{
    RkInt32            count = 0;
    GLFWvidmode const* modes = glfwGetVideoModes(in_handle, &count);

    for (RkInt32 i = 0; i < count; ++i)
    {
        m_video_modes.emplace_back(
            modes[i].width,
            modes[i].height,
            modes[i].redBits,
            modes[i].greenBits,
            modes[i].blueBits,
            modes[i].refreshRate
        );
    }

    glfwSetMonitorUserPointer(in_handle, this);
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

vk::Offset2D Screen::GetPosition() const noexcept
{
    vk::Offset2D position = {};

    glfwGetMonitorPos(m_handle, &position.x, &position.y);

    return position;
}

vk::Rect2D Screen::GetWorkArea() const noexcept
{
    vk::Rect2D area = {};

    glfwGetMonitorWorkarea(m_handle, &area.offset.x,
                                     &area.offset.y,
                                     reinterpret_cast<RkInt32*>(&area.extent.width),
                                     reinterpret_cast<RkInt32*>(&area.extent.height));

    return area;
}

vk::Extent2D Screen::GetPhysicalSize() const noexcept
{
    vk::Extent2D extent = {};

    glfwGetMonitorPhysicalSize(m_handle, reinterpret_cast<RkInt32*>(&extent.width),
                                         reinterpret_cast<RkInt32*>(&extent.height));

    return extent;
}

vk::Scale2D Screen::GetContentScale() const noexcept
{
    vk::Scale2D scale = {};

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

#pragma endregion