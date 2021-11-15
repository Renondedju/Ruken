
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
