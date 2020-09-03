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

#include "Time/ControlClock.hpp"
#include <ctime>

USING_RUKEN_NAMESPACE

ControlClock::ControlClock() noexcept:
    m_last_time                 {InternalClock::now()},
    m_control_time             {0.0f},
    m_corrected_control_time {0.0f},
    m_timer                     {},
    m_frequency                 {1.0f/60.0f}, // 60 ControlPoint() calls allowed per (1) second
    m_time_scale             {1.0f}
{}

RkVoid ControlClock::ControlPoint() noexcept
{
    // Updating the accumulator
    TimePoint const now = InternalClock::now();
    m_control_time        = std::chrono::duration<RkDouble, std::ratio<1, 1>>(now - m_last_time).count();

    RkDouble sleep_time = 0.0;

    if (m_control_time < m_frequency)
    {
        sleep_time = m_frequency - m_control_time;

        m_timer.SetTiming(static_cast<RkInt64>(sleep_time * 1e+7));
        m_timer.NSleep();
    }

    m_corrected_control_time = sleep_time + m_control_time;
    m_last_time = InternalClock::now();
}

RkVoid ControlClock::SetControlFrequency(RkFloat const in_frequency) noexcept
{
    m_frequency = in_frequency;
}

RkFloat ControlClock::GetControlTime() const noexcept
{
    return static_cast<RkFloat>(m_corrected_control_time * m_time_scale);
}

RkFloat ControlClock::GetUnscaledControlTime() const noexcept
{
    return static_cast<RkFloat>(m_corrected_control_time);
}
