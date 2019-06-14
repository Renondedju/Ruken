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
#include "FundamentalTypes.hpp"
#include "Containers/Queue.hpp"
#include "Threading/Synchronized.hpp"

#include <atomic>
#include <mutex>

BEGIN_DAEMON_NAMESPACE

template <typename TData_Type>
class TsLockQueue
{
	private:

		std::condition_variable			m_empty_notification;
		std::condition_variable			m_push_notification;
		std::atomic_bool				m_unlock_all;
		Synchronized<Queue<TData_Type>>	m_queue;

	public:

		#pragma region Constructors

		TsLockQueue();
		TsLockQueue(TsLockQueue const& in_copy)		= default;
		TsLockQueue(TsLockQueue&& in_move) noexcept = default;
		~TsLockQueue();

		#pragma endregion

		#pragma region Methods

		/**
		 * \brief Pushes a new object into the queue
		 * \tparam TData Type of the data
		 * \param in_data data to push
		 */
		template <typename... TData, typename = std::enable_if_t<std::is_constructible_v<TData_Type, TData...>>>
		DAEvoid Push(TData&... in_data);

		/**
		 * \brief Pops an object from the queue, if there is none, this method will lock the thread until a new object is added to the queue.
		 * \return Object
		 */
		[[nodiscard]]
		TData_Type Pop();
		
		/**
		 * \brief Releases all the locked threads from the pop() method
		 */
		DAEvoid Release();

		/**
		 * \brief Checks if the queue is empty
		 * \return True if the queue is currently empty, false otherwise
		 */
		[[nodiscard]]
		DAEbool Empty() noexcept;

		/**
		 * \brief Blocks the current thread until the queue gets emptied.
		 */
		DAEvoid WaitUntilEmpty();

		#pragma endregion

		#pragma region Operators

		TsLockQueue& operator=(TsLockQueue const& in_copy)		= default;
		TsLockQueue& operator=(TsLockQueue&& in_move) noexcept	= default;

		#pragma endregion
};

#include "Threading/TsLockQueue.inl"

END_DAEMON_NAMESPACE
