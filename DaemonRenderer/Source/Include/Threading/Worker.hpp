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

#include <thread>

#include "Config.hpp"

#include "Types/NonCopyable.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Containers/String.hpp"

BEGIN_DAEMON_NAMESPACE

class Worker : NonCopyable
{
	private:

		#pragma region Variables

		std::thread m_thread;

		#ifdef DAEMON_THREADING_ENABLE_THREAD_LABELS
			String m_label;
		#endif

		#pragma endregion

	public:

		#pragma region Constructors

		Worker(DAEchar const* in_label) noexcept;
		Worker()						noexcept;

		Worker(Worker const& in_copy)	noexcept = delete;
		Worker(Worker&&		 in_move)	noexcept = default;
		~Worker()						noexcept;

		#pragma endregion

		#pragma region Methods

		#ifdef DAEMON_THREADING_ENABLE_THREAD_LABELS

		/**
		 * \brief Label getter. If DAEMON_DISABLE_ENABLE_THREAD_LABELS is defined, then this method will return an empty string
		 * \return Worker's label
		 */
		[[nodiscard]]
		String const& Label() const noexcept;
		
		[[nodiscard]]
		String&		  Label() noexcept;

		#else

		/**
		 * \brief Label getter. If DAEMON_DISABLE_ENABLE_THREAD_LABELS is defined, then this method will return an empty string
		 * \return Worker's label
		 */
		[[nodiscard]]
		String Label() const noexcept;

		#endif

		/**
		 * \brief Checks if the worker is busy or not 
		 * \return True if the worker is busy
		 */
		[[nodiscard]]
		DAEbool Available() const noexcept;

		/**
		 * \brief Waits for the last task to execute, and starts the execution of this new job 
		 * \tparam TExecutable Type of the job
		 * \param in_job job to execute when done with the previous one
		 * \param in_args Args of the job to execute
		 */
		template <typename TExecutable, typename ...TArgs>
		DAEvoid Execute(TExecutable in_job, TArgs... in_args) noexcept;

		/**
		 * \brief Waits for the last task to execute, and starts the execution of this new job 
		 * \tparam TExecutable Type of the job
		 * \param in_job job to execute when done with the previous one
		 * \param in_args Args of the job to execute
		 */
		template <typename TExecutable, typename ...TArgs>
		DAEvoid ExecuteWithInstance(TExecutable in_job, TArgs... in_args) noexcept;

		/**
		 * \brief Locks the current thread until the current job has been done.
		 * \brief If there was no job currently executed, this method has no effect.
		 */
		DAEvoid WaitForAvailability() noexcept;

		/**
		 * \brief Separates the thread of execution from the worker object, allowing execution to continue independently.
		 * 
		 * \note Any allocated resources will be freed once the thread exits.
		 * \note If the worker was available, this method has no effect
		 */
		DAEvoid Detach() noexcept;

		/**
		 * \brief Returns the current underlying thread
		 * \return Worker's thread
		 */
		[[nodiscard]]
		std::thread& Thread() noexcept;

		#pragma endregion

		#pragma region Operators

		Worker& operator=(Worker const& in_copy) noexcept = delete;
		Worker& operator=(Worker&&		in_move) noexcept = default;

		#pragma endregion

};

#include "Threading/Worker.inl"

END_DAEMON_NAMESPACE
