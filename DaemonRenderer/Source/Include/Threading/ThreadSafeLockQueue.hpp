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

#include <mutex>
#include <atomic>

#include "Containers/Queue.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Threading/Synchronized.hpp"
#include "Threading/SynchronizedAccess.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Thread safe locking queue
 * \tparam TType Type contained into the queue
 */
template <typename TType>
class ThreadSafeLockQueue
{
	private:

		#pragma region Memebers

		std::condition_variable		m_empty_notification;
		std::condition_variable		m_push_notification;
		std::atomic_bool			m_unlock_all;
		std::mutex					m_empty_mutex;
		std::mutex					m_push_mutex;
		Synchronized<Queue<TType>>	m_queue;
	
		#pragma endregion 

		using QueueReadAccess  = typename decltype(m_queue)::ReadAccess;
		using QueueWriteAccess = typename decltype(m_queue)::WriteAccess;

	public:

		#pragma region Constructors

		ThreadSafeLockQueue();
		ThreadSafeLockQueue(ThreadSafeLockQueue const& in_copy)		= default;
		ThreadSafeLockQueue(ThreadSafeLockQueue&& in_move) noexcept = default;
		~ThreadSafeLockQueue();

		#pragma endregion 

		#pragma region Methods

		/**
		 * \brief Releases the queue
		 */
		DAEvoid Release();

		/**
		 * \brief Blocks the current thread until the queue gets emptied.
		 */
		DAEvoid WaitUntilEmpty();

		/**
		 * \brief Checks if the queue is empty
		 * \return True if the queue is empty, false otherwise
		 */
		DAEbool Empty() noexcept;

		/**
		 * \brief Enqueue an item
		 * \param in_item Item to enqueue
		 */
		DAEvoid Enqueue(TType&& in_item) noexcept;

		/**
		 * \brief Tries to dequeue an item, if the queue is empty,
		 * \brief the caller thread will be locked until the queue gets a new item or gets released
		 * 
		 * \param out_item Dequeued item
		 * \return True if the content of out_item is valid, false otherwise.
		 * 
		 * \note This method only returns false if Release() is called from another thread
		 */
		DAEbool Dequeue(TType& out_item) noexcept;

		#pragma endregion 

		#pragma region Operators

		ThreadSafeLockQueue& operator=(ThreadSafeLockQueue const& in_copy)		= default;
		ThreadSafeLockQueue& operator=(ThreadSafeLockQueue&& in_move) noexcept	= default;

		#pragma endregion 
};

#include "Threading/ThreadSafeLockQueue.inl"

END_DAEMON_NAMESPACE