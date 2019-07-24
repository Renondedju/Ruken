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

#include "Time/Clock.hpp"
#include <iostream>

USING_DAEMON_NAMESPACE

Clock::Clock() noexcept:
	m_last_time		{InternalClock::now()},
	m_accumulator	{0.0f},
	m_interval		{1.0f/60.0f}, // Default is 60 ticks/sec
	m_ticks			{0Ui64}
{}

DAEvoid Clock::SetFrequency(DAEfloat const in_interval) noexcept
{
	m_interval = in_interval;
}

DAEvoid Clock::Reset() noexcept
{
	m_accumulator = 0.0f;
	m_ticks		  = 0Ui64;
	m_last_time   = InternalClock::now();
}

DAEvoid Clock::Update() noexcept
{
	// Updating the accumulator
	TimePoint const now = InternalClock::now();

	m_accumulator += std::chrono::duration<decltype(m_accumulator), std::ratio<1>>(now - m_last_time).count();
	m_last_time = now;

	// Extracting ticks from the accumulator
	m_ticks += static_cast<DAEuint64>(m_accumulator / m_interval);
	m_accumulator = fmodf(m_accumulator, m_interval);
}

DAEuint64 Clock::ConsumeTick() noexcept
{
	if (!m_ticks)
		return 0;

	return --m_ticks;
}

DAEuint64 Clock::Ticks() const noexcept
{
	return m_ticks;
}
