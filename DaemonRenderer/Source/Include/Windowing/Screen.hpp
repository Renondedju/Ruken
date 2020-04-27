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

#include <vector>

#define GLFW_INCLUDE_NONE

#include <glfw/glfw3.h>

#include "Vector/Vector.hpp"

#include "Windowing/WindowParams.hpp"

BEGIN_DAEMON_NAMESPACE

struct VideoMode
{
    DAEint32 width;
    DAEint32 height;
    DAEint32 red_bits;
    DAEint32 green_bits;
    DAEint32 blue_bits;
    DAEint32 refresh_rate;
};

struct GammaRamp
{
    DAEuint16*  red;
    DAEuint16*  green;
    DAEuint16*  blue;
    DAEuint32   size;
};

/**
 * \brief Manages a currently connected monitor.
 */
class Screen
{
    /**
     * \brief WindowManager is the only class who can create a screen.
     */
    friend class WindowManager;

    private:

        #pragma region Members

        GLFWmonitor*            m_handle;
        std::string             m_name;
        VkOffset2D              m_position;
        VkRect2D                m_work_area;
        VkExtent2D              m_physical_size;
        Vector2f                m_content_scale;
        std::vector<VideoMode>  m_video_modes;

        #pragma endregion

        #pragma region Constructors

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit Screen(GLFWmonitor* in_handle);

        Screen(Screen const&    in_copy) = delete;
        Screen(Screen&&         in_move) = default;

        ~Screen() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return The opaque monitor object.
         */
        [[nodiscard]]
        GLFWmonitor* GetHandle() const noexcept;

        /**
         * \return The human-readable name, encoded as UTF-8, of the monitor.
         */
        [[nodiscard]]
        std::string const& GetName() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the specified monitor.
         */
        [[nodiscard]]
        VkOffset2D const& GetPosition() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the work area of the specified monitor
         *         along with the work area size in screen coordinates.
         * \note   The work area is defined as the area of the monitor not occluded by the operating system task bar where present.
         */
        [[nodiscard]]
        VkRect2D const& GetWorkArea() const noexcept;

        /**
         * \return The size, in millimeters, of the display area of the monitor.
         */
        [[nodiscard]]
        VkExtent2D const& GetPhysicalSize() const noexcept;

        /**
         * \return The content scale for the specified monitor.
         * \note   The content scale is the ratio between the current DPI and the platform's default DPI.
         */
        [[nodiscard]]
        Vector2f const& GetContentScale() const noexcept;

        /**
         * \return The current video mode of the specified monitor.
         */
        [[nodiscard]]
        VideoMode GetCurrentVideoMode() const noexcept;

        /**
         * \return The array of all video modes supported by the monitor.
         */
        [[nodiscard]]
        std::vector<VideoMode> const& GetVideoModes() const noexcept;

        /**
         * \return The current gamma ramp of the monitor.
         */
        [[nodiscard]]
        GammaRamp GetGammaRamp() const noexcept;

        /**
         * \brief This function generates an appropriately sized gamma ramp from the specified exponent and then sets it.
         * \note  The value must be a finite number greater than zero.
         */
        DAEvoid SetGamma(DAEfloat in_gamma) const noexcept;

        /**
         * \brief Sets the current gamma ramp for the monitor.
         */
        DAEvoid SetGammaRamp(GammaRamp const& in_gamma_ramp) const noexcept;

        #pragma endregion

        #pragma region Operators

        Screen& operator=(Screen const& in_copy) = delete;
        Screen& operator=(Screen&&      in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE