#pragma once

#include "Build/BuildInfo.hpp"
#include "Core/ExecutiveSystem/Concepts/AwaitableType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUCoroutineContinuation.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"

#include "Debug/Trace.hpp"

#include <tracy/TracyC.h>
#include <tracy/Tracy.hpp>
#include <source_location>

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TResult>
struct CPUTask;

/**
 * \brief Implements the base common behavior for all CPU tasks
 * \tparam TResult Return type of the associated coroutine
 */
template <QueueHandleType TQueueHandle, typename TResult>
struct CPUTaskPromise final: CPUAwaitable<TResult, false>, CPUAwaiter
{
    template <typename TOtherResult>
    friend class CPUPromise;

    template <typename TOtherResult, bool TOtherIsNoexcept>
    friend struct CPUCoroutineContinuation;

    using ProcessingUnit = CentralProcessingUnit;

    #pragma region Lifetime

    CPUTaskPromise()                      = default;
    CPUTaskPromise(CPUTaskPromise const&) = default;
    CPUTaskPromise(CPUTaskPromise&&     ) = default;
    ~CPUTaskPromise() override            = default;

    CPUTaskPromise& operator=(CPUTaskPromise const&) = default;
    CPUTaskPromise& operator=(CPUTaskPromise&&     ) = default;

	#pragma endregion

    #pragma region Methods

    /// ----- Coroutine methods -----
    ///

    /**
     * \brief Constructs, queues up and returns a handle to the promise
     * \return Promise handle
     */
    CPUTask<TQueueHandle, TResult> get_return_object() noexcept;

	/**
	 * CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed.
	 *
	 * @param in_location Source location at the start of the coroutine.
	 * @return Awaiter instance
	 */
	auto initial_suspend([[maybe_unused]] std::source_location const& in_location = std::source_location::current()) noexcept;

	/**
     * \brief Converts awaited types to asynchronous events if possible
     * \tparam TAwaitable Event type
     * \param in_awaitable Asynchronous event instance
     * \param in_source_location Source location of the transform
     * \return Awaiter instance
     */
    template <AwaitableType TAwaitable>
    auto await_transform(TAwaitable&&         in_awaitable,
        [[maybe_unused]] std::source_location in_source_location = std::source_location::current()
    ) noexcept;

	/**
     * Final suspension depends on the number of references that are made to the coroutine.
     * Since we have to hold a result, the promise cannot be destroyed if there are still references to it
     * in that case, the last reference to be removed will destroy the coroutine.
     *
	 * @return Awaiter instance
	 */
	auto final_suspend() noexcept;

	void unhandled_exception() noexcept;

	#pragma endregion

	private:

		#ifdef RUKEN_TRACE_BUILD
		std::source_location m_current_source_location {};
		TracyCZoneCtx        m_zone                    {};
		#endif

		#pragma region Methods

		/**
		 * \brief Called by the awaited event upon completion
		 * This method simply pushes the coroutine back to the queue for execution.
		 */
		RkVoid OnAwaitedContinuation() noexcept override
		{
			// CPU Tasks are not processed in place and are instead pushed to a queue
			// to be picked up and processed by a worker later.
			TQueueHandle::GetInstance().Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));
			//TracyMessageL("Pushed continuation");
		}

		/**
		 * \brief Destroys the coroutine frame when there is no longer any references made to it.
		 */
		RkVoid Deallocate() override
		{
			std::coroutine_handle<CPUTaskPromise>::from_promise(*this).destroy();
		}

		#pragma endregion
};

END_RUKEN_NAMESPACE

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUTaskPromise.inl"