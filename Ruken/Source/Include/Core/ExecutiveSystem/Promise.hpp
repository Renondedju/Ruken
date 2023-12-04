#pragma once

#include <exception>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueue, typename TReturnType>
using Task;

template <QueueHandleType TQueueHandle, typename TReturnType = RkVoid>
struct Promise
{
    using ProcessingUnit    = typename TQueueHandle::ProcessingUnit;
    using FinalSuspension   = std::suspend_never;
    using InitialSuspension = std::conditional_t<
        ProcessingUnit::execution_policy == EExecutionPolicy::Deferred,
        std::suspend_never,   // Submittable queues requires to be recorded and
        std::suspend_always  // thus needs to be executed directly by the caller
    >; 

    #pragma region Members

    TReturnType result;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Converts awaited types to asynchronous events if possible
     * \tparam TEvent Event type
     * \param in_awaitable Asynchronous event instance
     * \return Subscription instance
     */
    template <typename TEvent> requires AwaitableType<TEvent, TQueueHandle>
    auto await_transform(TEvent const& in_awaitable) noexcept
    {
        return in_awaitable.template GetSubscription<TQueueHandle>({std::coroutine_handle<Promise>::from_promise(*this)});
    }

    Task<TQueueHandle, TReturnType> get_return_object() noexcept;
    void                            return_value(TReturnType&& in_return_value) const noexcept {
        result = std::move(in_return_value);
    }

    [[noreturn]]
    RkVoid unhandled_exception();

    constexpr InitialSuspension initial_suspend() noexcept { return {}; }
    FinalSuspension final_suspend() noexcept;

    #pragma endregion
};

/**
 * The Promise object defines and controls the behaviour of the coroutine itself
 * by implementing methods that are called at specific points during execution of the coroutine.
 *
 * \tparam TQueueHandle The owning queue of the promise object
 */
template <QueueHandleType TQueueHandle>
struct Promise<TQueueHandle, RkVoid>
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
    template <typename TEvent> requires AwaitableType<TEvent, TQueueHandle>
    auto await_transform(TEvent const& in_awaitable) noexcept
    {
        return in_awaitable.template GetSubscription<TQueueHandle>({std::coroutine_handle<Promise>::from_promise(*this)});
    }

    Task<TQueueHandle, RkVoid> get_return_object() noexcept;
    void                       return_void() const noexcept {}

    [[noreturn]]
    RkVoid unhandled_exception();

    constexpr InitialSuspension initial_suspend() noexcept { return {}; }
    FinalSuspension final_suspend() noexcept;

    #pragma endregion
};

#include "Core/ExecutiveSystem/Promise.inl"

END_RUKEN_NAMESPACE