#pragma once

#include <exception>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/TaskCompletionEvent.hpp"
#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "Core/ExecutiveSystem/Concepts/AsynchronousEventType.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueue>
struct Task;

/**
 * The Promise object defines and controls the behaviour of the coroutine itself
 * by implementing methods that are called at specific points during execution of the coroutine.
 *
 * \tparam TQueueHandle The owning queue of the promise object
 */
template <QueueHandleType TQueueHandle>
struct Promise final: TaskCompletionEvent<typename TQueueHandle::ProcessingUnit>::Type
{
    using ProcessingUnit    = typename TQueueHandle::ProcessingUnit;
    using FinalSuspension   = std::suspend_never;
    using InitialSuspension = std::conditional_t<
        ProcessingUnit::execution_policy == EExecutionPolicy::Deferred,
        std::suspend_never,   // Submittable queues requires to be recorded and
        std::suspend_always  // thus needs to be executed directly by the caller
    >; 
        
    #pragma region Methods

    /**
     * \brief Converts awaited types to asynchronous events if possible
     * \tparam TEvent Event type
     * \param in_awaitable Asynchronous event instance
     * \return Subscription instance
     */
    template <typename TEvent> requires AsynchronousEventType<TEvent, TQueueHandle>
    auto await_transform(TEvent const& in_awaitable) noexcept
    {
        return in_awaitable.template GetSubscription<TQueueHandle>({std::coroutine_handle<Promise>::from_promise(*this)});
    }

    Task<TQueueHandle> get_return_object() noexcept;
    void               return_void() const noexcept {}

    [[noreturn]]
    RkVoid unhandled_exception();

    constexpr InitialSuspension initial_suspend() noexcept { return {}; }
    FinalSuspension final_suspend() noexcept;

    #pragma endregion
};

#include "Core/ExecutiveSystem/Promise.inl"

END_RUKEN_NAMESPACE