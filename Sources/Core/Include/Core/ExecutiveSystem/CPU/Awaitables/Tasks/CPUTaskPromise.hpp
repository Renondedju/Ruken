#pragma once

#include <source_location>

#include "Build/Namespace.hpp"
#include "Build/BuildInfo.hpp"
#include "Core/ExecutiveSystem/Concepts/AwaitableType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUCoroutineContinuation.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"

#include "Debug/Tracy.hpp"

#include <tracy/TracyC.h>
#include <tracy/Tracy.hpp>

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TResult>
struct CPUTask;

/**
 * \brief Implements the base common behavior for all CPU tasks
 * \tparam TResult Return type of the associated coroutine
 */
template <QueueHandleType TQueueHandle, typename TResult>
class CPUTaskPromise final:
    public CPUAwaitable<TResult, false>,
    public CPUAwaiter
{
    template <typename TOtherResult>
    friend class CPUPromise;

    template <typename TOtherResult, bool TOtherIsNoexcept>
    friend struct CPUCoroutineContinuation;

    #ifdef RUKEN_TRACE_BUILD
    std::source_location m_source_location {};
    TracyCZoneCtx        m_zone            {};
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

    public:

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
        CPUTask<TQueueHandle, TResult> get_return_object() noexcept
        {
            // The handle NEEDS to be initialized before the task is pushed
            // in the case we hold a result to make sure the reference counter
            // has time to be incremented to 1 before the task is executed and deleted by another thread
            CPUTask<TQueueHandle, TResult> handle {*this};

            return handle;
        }

        /**
         * \brief Converts awaited types to asynchronous events if possible
         * \tparam TAwaitable Event type
         * \param in_awaitable Asynchronous event instance
         * \return Subscription instance
         */
#ifdef RUKEN_TRACE_BUILD
        template <AwaitableType TAwaitable>
        auto await_transform(TAwaitable&& in_awaitable, std::source_location in_source_location = std::source_location::current()) noexcept
        {
            m_source_location = in_source_location;
#else
        template <AwaitableType TAwaitable>
        auto await_transform(TAwaitable&& in_awaitable) noexcept
        {
#endif
            using AResult              = typename std::decay_t<TAwaitable>::Result;
            using AProcessingUnit      = typename std::decay_t<TAwaitable>::ProcessingUnit;
            constexpr bool is_noexcept =          std::decay_t<TAwaitable>::reliable;
            static_assert(std::is_same_v<AProcessingUnit, CentralProcessingUnit>, 
                "Awaiting events from other processing units is not yet supported");

            struct Awaiter : CPUCoroutineContinuation<AResult, is_noexcept>
            {
                CPUTaskPromise& self;

                explicit Awaiter(CPUTaskPromise& in_self, CPUAwaitableHandle<AResult, is_noexcept>&& in_handle) noexcept:
                    CPUCoroutineContinuation<AResult, is_noexcept> {in_self, std::forward<CPUAwaitableHandle<AResult, is_noexcept>>(in_handle)},
                    self    (in_self)
                {}

                auto await_ready() noexcept
                {
                    #ifdef RUKEN_TRACE_BUILD
                    TracyCZoneEnd(self.m_zone);
                    #endif
                    return CPUCoroutineContinuation<AResult, is_noexcept>::await_ready();
                }

                auto await_suspend(std::coroutine_handle<> in_handle) noexcept
                {
                    return CPUCoroutineContinuation<AResult, is_noexcept>::await_suspend(in_handle);
                }

                auto await_resume() const
                {
                    #ifdef RUKEN_TRACE_BUILD
                    TracySCZone(self.m_zone, self.m_source_location, true);
                    #endif

                    if constexpr (std::is_same_v<AResult, RkVoid>)
                        CPUCoroutineContinuation<AResult, is_noexcept>::await_resume();
                    else
                        return CPUCoroutineContinuation<AResult, is_noexcept>::await_resume();
                }
            };

            // In the case we don't need a bridge, we know the awaitable inherits from CPUAwaitable
            if constexpr(std::is_base_of_v<CPUAwaitableHandle<AResult, is_noexcept>, TAwaitable>)
                return Awaiter {*this, std::forward<TAwaitable>(in_awaitable)};
            else
                return Awaiter {*this, CPUAwaitableHandle<AResult, is_noexcept>(in_awaitable)};
        }

        // CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed.
        // Final suspension depends on the number of references that are made to the coroutine.
        // Since we have to hold a result, the promise cannot be destroyed if there are still references to it
        // in that case, the last reference to be removed will destroy the coroutine.
        // If no references are made to the coroutine at the time of completion, the destruction happens immediately.
#ifdef RUKEN_TRACE_BUILD
        auto initial_suspend(std::source_location const& in_location = std::source_location::current()) noexcept
        {
            m_source_location = in_location;
#else
        auto initial_suspend() noexcept
        {
#endif
            struct Awaiter
            {
                CPUTaskPromise& self;

                [[nodiscard]]
                bool await_ready() const noexcept
                {
                    auto value {WorkerInfo::current_queue == &TQueueHandle::GetInstance() && WorkerInfo::remaining_tasks >= 1};
                    if (value)
                        --WorkerInfo::remaining_tasks;

                    return value;
                }

                void await_suspend(std::coroutine_handle<>) const noexcept
                {
                    // CPU Tasks are not processed in place and are instead pushed to a queue
                    // to be picked up and processed by a worker later.
                    TQueueHandle::GetInstance().Push(std::coroutine_handle<CPUTaskPromise>::from_promise(self));
                }

                void await_resume() const noexcept
                {
                    #ifdef RUKEN_TRACE_BUILD
                    TracySCZone(self.m_zone, self.m_source_location, true);
                    #endif
                }
            };

            return Awaiter {*this};
        }

        auto final_suspend() noexcept
        {
            struct Awaiter: std::suspend_always
            {
                CPUTaskPromise& self;

                void await_suspend(std::coroutine_handle<>) const noexcept
                {
                    #ifdef RUKEN_TRACE_BUILD
                    TracyCZoneEnd(self.m_zone);
                    #endif

                    self.SignalConsume          ();
                    self.DecrementReferenceCount();
                }
            };

            return Awaiter {{}, *this};
        }

		void unhandled_exception() noexcept
        { this->Cancel(std::current_exception()); }

        #pragma endregion
};

END_RUKEN_NAMESPACE