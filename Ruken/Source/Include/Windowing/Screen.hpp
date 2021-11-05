
#pragma once

#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include "Windowing/GammaRamp.hpp"
#include "Windowing/VideoMode.hpp"

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;

/**
 * \brief Manages a currently connected monitor.
 */
class Screen
{
    /**
     * \brief WindowManager is the only class able to create a screen.
     */
    friend class WindowManager;

    private:

        #pragma region Members

        Logger*                m_logger      {nullptr};
        GLFWmonitor*           m_handle      {nullptr};
        std::string            m_name        {};
        std::vector<VideoMode> m_video_modes {};

        #pragma endregion

        #pragma region Constructors

        explicit Screen(Logger* in_logger, GLFWmonitor* in_handle) noexcept;

        #pragma endregion

        #pragma region Methods

        static Screen* GetMonitorUserPointer(GLFWmonitor* in_monitor) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        Screen() = delete;

        Screen(Screen const& in_copy) = delete;
        Screen(Screen&&      in_move) = delete;

        ~Screen() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] GLFWmonitor*                  GetHandle    () const noexcept;
        [[nodiscard]] std::string            const& GetName      () const noexcept;
        [[nodiscard]] std::vector<VideoMode> const& GetVideoModes() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the monitor.
         */
        [[nodiscard]]
        vk::Offset2D GetPosition() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the work area of the monitor
         *         along with the work area size in screen coordinates.
         * \note   The work area is defined as the area of the monitor not occluded by the operating system task bar where present.
         *         If no task bar exists then the work area is the monitor resolution in screen coordinates.
         */
        [[nodiscard]]
        vk::Rect2D GetWorkArea() const noexcept;

        /**
         * \return The size, in millimeters, of the display area of the monitor.
         * \note   Some systems do not provide accurate monitor size information,
         *         either because the monitor EDID data is incorrect or because the driver does not report it accurately.
         */
        [[nodiscard]]
        vk::Extent2D GetPhysicalSize() const noexcept;

        /**
         * \brief  The content scale is the ratio between the current DPI and the platform's default DPI.
         *         This is especially important for text and any UI elements.
         * \return The content scale of the monitor.
         * \note   The content scale may depend on both the monitor resolution and pixel density and on user settings.
         *         It may be very different from the raw DPI calculated from the physical size and current resolution.
         */
        [[nodiscard]]
        vk::Scale2D GetContentScale() const noexcept;

        /**
         * \return The current video mode of the specified monitor.
         * \note   If you have created a full screen window for that monitor, the return value will depend on whether that window is iconified.
         */
        [[nodiscard]]
        VideoMode GetCurrentVideoMode() const noexcept;

        /**
         * \return The current gamma ramp of the monitor.
         */
        [[nodiscard]]
        GammaRamp GetGammaRamp() const noexcept;

        /**
         * \brief This function generates an appropriately sized gamma ramp from the specified exponent and then sets it like "SetGammaRamp".
         * \note  The value must be a finite number greater than zero.
         */
        RkVoid SetGamma(RkFloat in_gamma) const noexcept;

        /**
         * \brief Sets the current gamma ramp for the monitor.
         * \note  The software controlled gamma ramp is applied in addition to the hardware gamma correction,
         *        which today is usually an approximation of sRGB gamma.
         *        This means that setting a perfectly linear ramp, or gamma 1.0, will produce the default (usually sRGB-like) behavior.
         */
        RkVoid SetGammaRamp(GammaRamp const& in_gamma_ramp) const noexcept;

        #pragma endregion

        #pragma region Operators

        Screen& operator=(Screen const& in_copy) = delete;
        Screen& operator=(Screen&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE