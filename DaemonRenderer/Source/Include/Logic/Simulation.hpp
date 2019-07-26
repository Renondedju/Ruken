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

#include "Config.hpp"
#include "Time/ControlClock.hpp"

BEGIN_DAEMON_NAMESPACE

class Simulation
{
	private:

		#pragma region Members

		// This clock is responsible for the simulation update speed
		ControlClock m_simulation_clock;

		// This clock is responsible for the render speed of the simulation and is driven by the renderer
		ControlClock m_render_clock;

		#pragma endregion

	public:
		
		#pragma region Constructors

		Simulation()								= default;
		Simulation(Simulation const& in_copy)		= default;
		Simulation(Simulation&& in_move) noexcept	= default;
		~Simulation()								= default;
		
		#pragma endregion

		#pragma region Methods

		/**
		 * \brief Returns the simulation clock, this allows for a full control over any updates
		 * \note This clock does not control the rendering of the simulation
		 * 
		 * \return Simulation clock
		 */
		ControlClock& GetClock() noexcept;

		#pragma endregion

		#pragma region Operators

		Simulation& operator=(Simulation const& in_copy) noexcept = default;
		Simulation& operator=(Simulation&&		in_move) noexcept = default;

		#pragma endregion
};

END_DAEMON_NAMESPACE