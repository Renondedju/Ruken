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

#include "Vulkan/Vulkan.hpp"
#include <vector>

#include "Utility.hpp"

#include "Vector/Vector.hpp"

#include "Containers/String.hpp"
#include "Containers/Vector.hpp"

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
 * \brief This class manages a currently connected monitor.
 */
class Screen
{
    /**
     * \brief WindowManager is the only class who can modify a screen.
     */
    friend class WindowManager;

    private:

        #pragma region Members

        GLFWmonitor*        m_handle;
        std::string            m_name;
        VkExtent2D          m_physical_size;
        Vector2f            m_content_scale;
        VkOffset2D          m_position;
        VkRect2D            m_work_area;
        std::vector<VideoMode> m_video_modes;
        Area2D                 m_work_area;
        Position2D             m_position;
        GLFWmonitor*           m_handle;
        std::string            m_name;

        #pragma endregion

        #pragma region Constructors

        explicit Screen(GLFWmonitor* in_handle);

        #pragma endregion

    public:

        #pragma region Constructors

        Screen(Screen const&    in_other) = delete;
        Screen(Screen&&         in_other) = default;
        ~Screen()                         = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return The opaque monitor object.
         */
        [[nodiscard]]
        GLFWmonitor* GetHandle() const noexcept;

        /**
         * \return The human-readable of the monitor.
         */
        [[nodiscard]]
        std::string const& GetName() const noexcept;

        /**
         * \return The size, in millimeters, of the display area of the monitor.
         */
        [[nodiscard]] VkExtent2D const& GetPhysicalSize() const noexcept;

        /**
         * \brief The content scale is the ratio between the current DPI and the platform's default DPI.
         * \return The content scale of the monitor.
         */
        [[nodiscard]] Vector2f const& GetContentScale() const noexcept;

        /**
         * \return The position, in screen coordinates, of the upper-left corner of the monitor.
         */
        [[nodiscard]] VkOffset2D const& GetPosition() const noexcept;

        /**
         * \brief The work area is defined as the area of the monitor not occluded by the operating system task bar where present.
         * \return The position, in screen coordinates, of the upper-left corner of
         *         the work area of the specified monitor along with the work area size in screen coordinates.
         * \note If no task bar exists then the work area is the monitor resolution in screen coordinates.
         */
        [[nodiscard]] VkRect2D const& GetWorkArea() const noexcept;

        /**
         * \return The current video mode of the specified monitor.
         * \note If you have created a full screen window for that monitor,
         *       the return value will depend on whether that window is iconified.
         */
        [[nodiscard]]
        VideoMode const& GetCurrentVideoMode() const noexcept;

        /**
         * \return The array of all video modes supported by the monitor.
         * \note The returned array is sorted in ascending order,
         *       first by color bit depth (the sum of all channel depths)
         *       and then by resolution area (the product of width and height).
         */
        [[nodiscard]]
        std::vector<VideoMode> const& GetVideoModes() const noexcept;

        /**
         * \return The current gamma ramp of the monitor.
         */
        [[nodiscard]]
        GammaRamp const& GetGammaRamp() const noexcept;

        /**
         * \brief This function generates an appropriately sized gamma ramp from the specified exponent and then sets it.
         * \param in_gamma The new exponent.
         * \note The value must be a finite number greater than zero.
         */
        DAEvoid SetGamma(DAEfloat in_gamma) const noexcept;

        /**
         * \brief Sets the current gamma ramp for the monitor.
         * \param in_gamma_ramp The new gamma ramp.
         */
        DAEvoid SetGammaRamp(GammaRamp const& in_gamma_ramp) const noexcept;

        #pragma endregion

        #pragma region Operators

        Screen& operator=(Screen const& in_other) = delete;
        Screen& operator=(Screen&&      in_other) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE