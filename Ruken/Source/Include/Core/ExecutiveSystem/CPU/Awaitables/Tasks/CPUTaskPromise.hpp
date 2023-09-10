#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/Concepts/AwaitableType.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUCoroutineContinuation.hpp"

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

    struct FinalSuspension
    {
        CPUTaskPromise const& promise;

        [[nodiscard]]
        bool await_ready() const noexcept {
            return promise.GetReferenceCount() == 0;
        }

        constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
        constexpr void await_resume() const noexcept {}
    };

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

    public:

        using ProcessingUnit = CentralProcessingUnit;

        #pragma region Methods

        /// ----- Coroutine methods -----
        ///

        /**
         * \brief Constructs, queues up and returns a handle to the promise
         * \return Promise handle
         */
        CPUTask<TQueueHandle, TReturnValue> get_return_object() noexcept
        {
            // CPU Tasks are not processed in place and are instead pushed to a queue
            // to be picked up and processed by a worker later.
            TQueueHandle::instance.Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));

            return {*this};
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
            if constexpr(std::is_same_v<AReturnType, RkVoid>)
                return CPUCoroutineContinuation<RkVoid> (in_awaitable.m_continuation_hook, *this);
            else
                return CPUCoroutineContinuation<AReturnType> (
                    in_awaitable.m_result, in_awaitable.m_continuation_hook, *this
                );
        }

        /**
         * \brief Called when the coroutine returns a value
         * \param in_result Result emitted by the coroutine
         */
        void return_value(TReturnValue&& in_result) noexcept
        {   CPUAwaitable<TReturnValue>::m_result = std::forward<TReturnValue>(in_result); }

        void return_value(TReturnValue const& in_result) noexcept
            requires !std::is_reference_v<TReturnValue>
        {   CPUAwaitable<TReturnValue>::m_result = in_result; }

        // CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed.
        // Final suspension depends on the number of references that are made to the coroutine.
        // Since we have to hold a result, the promise cannot be destroyed if there are still references to it
        // in that case, the last reference to be removed will destroy the coroutine.
        // If no references are made to the coroutine at the time of completion, the destruction happens immediately.
        auto initial_suspend() noexcept {                           return std::suspend_always {};      }
        auto final_suspend  () noexcept { this->SignalCompletion(); return FinalSuspension {*this}; }

        void unhandled_exception() noexcept { std::terminate(); }

        #pragma endregion
};

/**
 * \brief Implements the base common behavior for all CPU tasks
 */
template <QueueHandleType TQueueHandle>
class CPUTaskPromise<TQueueHandle, RkVoid> final:
    public CPUAwaitable<RkVoid>,
    public CPUAwaiter
{
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

    public:

        #pragma region Methods

        /// ----- Coroutine methods -----
        ///

        /**
         * \brief Constructs, queues up and returns a handle to the promise
         * \return Promise handle
         */
        CPUTask<TQueueHandle, RkVoid> get_return_object() noexcept
        {
            // CPU Tasks are not processed in place and are instead pushed to a queue
            // to be picked up and processed by a worker later.
            TQueueHandle::instance.Push(std::coroutine_handle<CPUTaskPromise>::from_promise(*this));

            return {*this};
        }

        /**
         * \brief Called when the task awaits some event.
         *        Converts awaited types to asynchronous events if possible.
         *
         * \tparam TAwaitable Event type
         * \param in_awaitable Asynchronous event instance
         * \return Subscription instance
         */
        template <typename TAwaitable>
        auto await_transform(TAwaitable&& in_awaitable) noexcept
        {
            using AReturnType     = typename std::decay_t<TAwaitable>::ReturnType;
            using AProcessingUnit = typename std::decay_t<TAwaitable>::ProcessingUnit;
            static_assert(std::is_same_v<AProcessingUnit, CentralProcessingUnit>, 
                "Awaiting events from other processing units is not yet supported");

            // In the case we don't need a bridge, we know the awaitable inherits from CPUAwaitable
            if constexpr(std::is_same_v<AReturnType, RkVoid>)
                return CPUCoroutineContinuation<RkVoid> (in_awaitable.m_continuation_hook, *this);
            else
                return CPUCoroutineContinuation<AReturnType> (
                    in_awaitable.m_result, in_awaitable.m_continuation_hook, *this
                );
        }

        // A promise with a return type of void will return nothing
        // thanks to that, this specialization can get rid of the reference tracking
        // and destroy itself as soon as final_suspend has been called
        void return_void() const noexcept {}

        // CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed. 
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never  final_suspend  () noexcept { this->SignalCompletion(); return {}; }

        void unhandled_exception() noexcept { std::terminate(); }

        #pragma endregion
};

END_RUKEN_NAMESPACE