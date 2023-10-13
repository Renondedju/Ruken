#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/Concepts/AwaitableType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUCoroutineContinuation.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Utils/ConditionalSuspension.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType>
struct CPUTask;

/**
 * \brief Implements the base common behavior for all CPU tasks
 * \tparam TReturnValue Return type of the associated coroutine
 */
template <QueueHandleType TQueueHandle, typename TReturnValue>
class CPUTaskPromise final:
    public CPUAwaitable<TReturnValue>,
    public CPUAwaiter
{
    template <typename TOtherReturnType>
    friend class CPUPromise;

    /**
     * \brief Called by the awaited event upon completion
     * This method simply pushes the coroutine back to the queue for execution.
     */
    RkVoid OnContinuation() noexcept override
    {
        // CPU Tasks are not processed in place and are instead pushed to a queue
        // to be picked up and processed by a worker later.
        TQueueHandle::instance.Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));
    }

    /**
     * \brief Destroys the coroutine frame when there is no longer any references made to it.
     */
    RkVoid Deallocate() override
    {
        //std::coroutine_handle<CPUTaskPromise>::from_promise(*this).destroy();
    }

    public:

        using ProcessingUnit = CentralProcessingUnit;

        #pragma region Lifetime

        CPUTaskPromise()                      = default;
        CPUTaskPromise(CPUTaskPromise const&) = default;
        CPUTaskPromise(CPUTaskPromise&&     ) = default;
        ~CPUTaskPromise() override            = default;

        CPUTaskPromise& operator=(CPUTaskPromise const&) = default;
        CPUTaskPromise& operator=(CPUTaskPromise&&     ) = default;


        #pragma region Methods

        /// ----- Coroutine methods -----
        ///

        /**
         * \brief Constructs, queues up and returns a handle to the promise
         * \return Promise handle
         */
        CPUTask<TQueueHandle, TReturnValue> get_return_object() noexcept
        {
            // The handle NEEDS to be initialized before the task is pushed
            // in the case we hold a result to make sure the reference counter
            // has time to be incremented to 1 before the task is executed and deleted by another thread
            CPUTask<TQueueHandle, TReturnValue> handle {*this};

            // CPU Tasks are not processed in place and are instead pushed to a queue
            // to be picked up and processed by a worker later.
            TQueueHandle::instance.Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));

            return handle;
        }

        /**
         * \brief Converts awaited types to asynchronous events if possible
         * \tparam TAwaitable Event type
         * \param in_awaitable Asynchronous event instance
         * \return Subscription instance
         */
        template <AwaitableType TAwaitable>
        auto await_transform(TAwaitable&& in_awaitable) noexcept
        {
            using AReturnType     = typename std::decay_t<TAwaitable>::ReturnType;
            using AProcessingUnit = typename std::decay_t<TAwaitable>::ProcessingUnit;
            static_assert(std::is_same_v<AProcessingUnit, CentralProcessingUnit>, 
                "Awaiting events from other processing units is not yet supported");

            // In the case we don't need a bridge, we know the awaitable inherits from CPUAwaitable
            if constexpr(std::is_base_of_v<CPUAwaitableHandle<AReturnType>, TAwaitable>)
                return CPUCoroutineContinuation<AReturnType> (*this, in_awaitable);
            else
                return CPUCoroutineContinuation<AReturnType> (*this, CPUAwaitableHandle<AReturnType>(in_awaitable));
        }

        // CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed.
        // Final suspension depends on the number of references that are made to the coroutine.
        // Since we have to hold a result, the promise cannot be destroyed if there are still references to it
        // in that case, the last reference to be removed will destroy the coroutine.
        // If no references are made to the coroutine at the time of completion, the destruction happens immediately.
        auto initial_suspend() noexcept { return std::suspend_always {}; }
        auto final_suspend  () noexcept
        {
            RkBool is_orphaned = false;

            this->SignalCompletion ();
            this->DecrementReferenceCount(false, is_orphaned);

            return ConditionalSuspension {
                .condition = is_orphaned
            };
        }

        void unhandled_exception() noexcept { std::terminate(); }

        #pragma endregion
};

END_RUKEN_NAMESPACE