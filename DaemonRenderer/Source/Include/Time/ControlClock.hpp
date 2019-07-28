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

#pragma once

#include <chrono>

#include "Config.hpp"
#include "Time/Timer.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

class ControlClock
{
	using InternalClock = std::chrono::steady_clock;
	using TimePoint		= std::chrono::time_point<InternalClock>;

	private:

		#pragma region Members

		// Last time an update has been triggered
		TimePoint m_last_time;

		// Measured time between 2 ControlPoint() calls
		DAEdouble m_control_time;
		DAEdouble m_corrected_control_time;

		// Timer, this is used to sleep if required
		Timer m_timer;

		// Maximum frequency allowed between 2 ControlPoint() calls
		DAEfloat m_frequency;

		// Time scale of the clock, this can be used to temporarily
		// slow down or speed up the clock without having to modify the frequency of the clock
		// Be careful since this also multiplies the impressions of the clock
		DAEfloat m_time_scale;

		#pragma endregion

	public:

		#pragma region Constructors

		ControlClock() noexcept;
		ControlClock(ControlClock const& in_copy)	noexcept = default;
		ControlClock(ControlClock&&		 in_move)	noexcept = default;
		~ControlClock()								noexcept = default;

		#pragma endregion

		#pragma region Methods

		/**
		 * \brief Main method, must be called every time you wish to be synchronized with the clock
		 * \note If you are too slow between 2 clock cycles, this method has no effect
		 */
		DAEvoid ControlPoint() noexcept;

		/**
		 * \brief Sets the target frequency of the clock
		 * \param in_frequency frequency of the clock (number of seconds / cycle count)
		 */
		DAEvoid  SetControlFrequency(DAEfloat in_frequency) noexcept;

		/**
		 * \brief Queries the time spent between 2 ControlPoint() calls
		 * \return Control time
		 */
		DAEfloat GetControlTime() const noexcept;

		/**
		 * \brief Queries the time spent between 2 ControlPoint() calls and omits the time scale of the clock
		 * \return Unscaled control time
		 */
		DAEfloat GetUnscaledControlTime() const noexcept;

		#pragma endregion

		#pragma region Operators

		ControlClock& operator=(ControlClock const& in_copy) noexcept = default;
		ControlClock& operator=(ControlClock&&	    in_move) noexcept = default;

		#pragma endregion
};

END_DAEMON_NAMESPACE