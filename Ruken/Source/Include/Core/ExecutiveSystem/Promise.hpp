#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

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
class Promise
{
    using ProcessingUnit    = typename TQueue::ProcessingUnit;
    using FinalSuspension   = std::suspend_never;
    using InitialSuspension = std::conditional_t<
        ProcessingUnit::execution_method == EExecutionPolicy::Deferred,
        std::suspend_never,   // Submittable queues requires to be recorded and
        std::suspend_always  // thus needs to be executed directly by the caller
    >; 

    public:

        #pragma region Methods

        //template <ProcessingQueueType TAwaitedQueue>
        //Awaiter<TQueue, TAwaitedQueue> await_transform(AsynchronousEvent<TAwaitedQueue>&& in_awaited_event) noexcept;

        Task<TQueue> get_return_object() noexcept;
        void         return_void() const noexcept {}

        [[noreturn]]
        static RkVoid unhandled_exception();

        static constexpr InitialSuspension initial_suspend() noexcept { return {}; }
        static constexpr FinalSuspension   final_suspend  () noexcept { return {}; }

        #pragma endregion
};

#include "Core/ExecutiveSystem/Promise.inl"

END_RUKEN_NAMESPACE