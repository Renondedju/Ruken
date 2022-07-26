#pragma once

#include <exception>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/TaskCompletionEvent.hpp"
#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"
#include "Core/ExecutiveSystem/Concepts/AsynchronousEventType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingQueueType TQueue>
struct Task;

/**
 * The Promise object defines and controls the behaviour of the coroutine itself
 * by implementing methods that are called at specific points during execution of the coroutine.
 *
 * \tparam TQueue The owning queue of the promise object
 */
template <ProcessingQueueType TQueue>
struct Promise final: TaskCompletionEvent<typename TQueue::ProcessingUnit>::Type
{
    using ProcessingUnit    = typename TQueue::ProcessingUnit;
    using FinalSuspension   = std::suspend_never;
    using InitialSuspension = std::conditional_t<
        ProcessingUnit::execution_policy == EExecutionPolicy::Deferred,
        std::suspend_never,   // Submittable queues requires to be recorded and
        std::suspend_always  // thus needs to be executed directly by the caller
    >; 

    public:
        
        #pragma region Methods

        /**
         * \brief Converts awaited types to asynchronous events if possible
         * \tparam TEvent Event type
         * \param in_awaitable Asynchronous event instance
         * \return Subscription instance
         */
        template <typename TEvent> requires AsynchronousEventType<TEvent, TQueue>
        auto await_transform(TEvent const& in_awaitable) noexcept
        {
            return in_awaitable.template GetSubscription<TQueue>({std::coroutine_handle<Promise<TQueue>>::from_promise(*this)});
        }

        Task<TQueue> get_return_object() noexcept;
        void         return_void() const noexcept {}

        [[noreturn]]
        static RkVoid unhandled_exception();

        static constexpr InitialSuspension initial_suspend() noexcept { return {}; }
        FinalSuspension final_suspend() noexcept;

        #pragma endregion
};

#include "Core/ExecutiveSystem/Promise.inl"

END_RUKEN_NAMESPACE