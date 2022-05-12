#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/AsynchronousEvent.hpp"
#include "Core/ExecutiveSystem/Concepts/AwaiterType.hpp"

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
class Promise final: public AsynchronousEvent<TQueue>
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
         * \brief Generates an awaiter from an asynchronous event.
         * If no valid awaiter is found for both of these queue types, then the method
         * will be disabled and co_awaiting such an event will prevent compilation.
         *
         * \tparam TAwaitedQueue Queue type this promise will wait on
         * \param in_awaited_event Awaited event
         * \return Awaiter instance
         */
        template <ProcessingQueueType TAwaitedQueue> requires AwaiterType<Subscription<TQueue, TAwaitedQueue>>
        Subscription<TQueue, TAwaitedQueue> await_transform(AsynchronousEvent<TAwaitedQueue>& in_awaited_event) noexcept;

        template <ProcessingQueueType TAwaitedQueue> requires AwaiterType<Subscription<TQueue, TAwaitedQueue>>
        Subscription<TQueue, TAwaitedQueue> await_transform(Task<TAwaitedQueue> const& in_awaited_task) noexcept;

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