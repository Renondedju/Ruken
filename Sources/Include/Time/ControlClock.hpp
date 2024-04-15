
#pragma once

#include <chrono>

#include "Build/Namespace.hpp"

#include "Time/Timer.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class ControlClock
{
    using InternalClock = std::chrono::steady_clock;
    using TimePoint     = std::chrono::time_point<InternalClock>;

    private:

        #pragma region Members

        // Last time an update has been triggered
        TimePoint m_last_time;

        // Measured time between 2 ControlPoint() calls
        RkDouble m_control_time;
        RkDouble m_corrected_control_time;

        // Timer, this is used to sleep if required
        Timer m_timer;

        // Maximum frequency allowed between 2 ControlPoint() calls
        RkFloat m_frequency;

        // Time scale of the clock, this can be used to temporarily
        // slow down or speed up the clock without having to modify the frequency of the clock
        // Be careful since this also multiplies the impressions of the clock
        RkFloat m_time_scale;

        #pragma endregion

    public:

        #pragma region Constructors

        ControlClock ()                            noexcept;
        ControlClock (ControlClock const& in_copy) noexcept = default;
        ControlClock (ControlClock&&      in_move) noexcept = default;
        ~ControlClock()                            noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Main method, must be called every time you wish to be synchronized with the clock
         * \note If you are too slow between 2 clock cycles, this method has no effect
         */
        RkVoid ControlPoint() noexcept;

        /**
         * \brief Sets the target frequency of the clock
         * \param in_frequency frequency of the clock (number of seconds / cycle count)
         */
        RkVoid SetControlFrequency(RkFloat in_frequency) noexcept;

        /**
         * \brief Queries the time spent between 2 ControlPoint() calls
         * \return Control time
         */
        RkFloat GetControlTime() const noexcept;

        /**
         * \brief Queries the time spent between 2 ControlPoint() calls and omits the time scale of the clock
         * \return Unscaled control time
         */
        RkFloat GetUnscaledControlTime() const noexcept;

        #pragma endregion

        #pragma region Operators

        ControlClock& operator=(ControlClock const& in_copy) noexcept = default;
        ControlClock& operator=(ControlClock&&      in_move) noexcept = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE