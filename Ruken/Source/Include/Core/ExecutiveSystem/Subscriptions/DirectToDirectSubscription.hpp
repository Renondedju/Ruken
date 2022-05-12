#pragma once

#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/Subscription.hpp"
#include "Core/ExecutiveSystem/AsynchronousEvent.hpp"
#include "Core/ExecutiveSystem/Subscriptions/DirectSubscriptionBase.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TOwningQueue, typename TAwaitedQueue>
concept DirectToDirectQueue = 
    ProcessingQueueType<TOwningQueue > && TOwningQueue ::instruction_type == EInstructionType::Direct &&
    ProcessingQueueType<TAwaitedQueue> && TAwaitedQueue::instruction_type == EInstructionType::Direct;

/**
 * \brief Direct instruction to direct instruction subscription
 */
template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue> requires DirectToDirectQueue<TOwningQueue, TAwaitedQueue>
class Subscription<TOwningQueue, TAwaitedQueue> final: public DirectSubscriptionBase
{
    #pragma region Members

    // Owning task, used to resume it once the wait has been completed
    Task<TOwningQueue> const& m_owning;
    
    #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning Owning task instance
         * \param in_awaited Awaited event instance
         */
        Subscription(Task<TOwningQueue> const& in_owning, AsynchronousEvent<TAwaitedQueue>& in_awaited) noexcept;

        Subscription(Subscription const&) = delete;
        Subscription(Subscription&&)      = delete;
        ~Subscription() override          = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Called when the wait has been completed
         */
        RkVoid OnCompletion() noexcept override;

        #pragma endregion

        #pragma region Operators

        Subscription& operator=(Subscription const&) = delete;
        Subscription& operator=(Subscription&&)      = delete;

        #pragma endregion
};

#include "Core/ExecutiveSystem/Awaiters/DirectToDirectSubscription.inl"

END_RUKEN_NAMESPACE