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
#include "Containers/Array.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Threading/ThreadSafeQueue.hpp"

#include <functional>

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class is responsible for the repartition of different tasks between workers
 */
class Scheduler
{
	using Job = std::function<DAEvoid()>;

	private:

		#pragma region Memebers


		#pragma endregion

	public:

		#pragma region Constructors

		Scheduler()								= default;
		Scheduler(Scheduler const& in_copy)		= default;
		Scheduler(Scheduler&& in_move) noexcept = default;
		~Scheduler()							= default;

		#pragma endregion

		#pragma region Methods

		/**
		 * \brief Schedules a task on one of the available threads
		 * \param in_task Task to schedule, any return value will be discarded
		 */
		DAEvoid ScheduleTask(Job in_task) noexcept;

		#pragma endregion 

		#pragma region Operators

		Scheduler& operator=(Scheduler const& in_copy)		= default;
		Scheduler& operator=(Scheduler&& in_move) noexcept	= default;

		#pragma endregion
};

END_DAEMON_NAMESPACE