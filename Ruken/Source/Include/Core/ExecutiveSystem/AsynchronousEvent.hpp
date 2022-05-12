#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"
#include "Core/ExecutiveSystem/Subscriptions/DirectSubscriptionBase.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Asynchronous event that can be waited on and subscribed to.
 * Events implements the "when" question, they are responsible for designating
 * a given point in time until an awaiter is supposed to wait.
 *
 * \tparam TQueue Queue type the event lives in
 */
template <ProcessingQueueType TQueue>
class AsynchronousEvent
{};

/**
 * \brief Asynchronous event that can be waited on and subscribed to.
 * Events implements the "when" question, they are responsible for designating
 * a given point in time until an awaiter is supposed to wait.
 *
 * \tparam TQueue Queue type the event lives in
 */
template <ProcessingQueueType TQueue> requires (TQueue::instruction_type == EInstructionType::Direct)
class AsynchronousEvent<TQueue>
{
    template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue>
    friend class Subscription;

    #pragma region Members

    DirectSubscriptionBase::Node m_head {nullptr};

    #pragma endregion

    protected:

        #pragma region Methods

        /**
         * \brief Signals the completion of the event to every of the attached awaiters.
         * This will consume the event and make it completed until Reset() has been called.
         */
        RkVoid SignalCompletion() noexcept;

        /**
         * \brief Resets the event, making it non completed and ready to hook new awaiters.
         * Do note that any awaiting event will be lost and won't resume ever if reset is called when
         * Completed() returns false. Make sure to signal a completion before resetting.
         */
        RkVoid Reset() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        AsynchronousEvent ()                         = default;
        AsynchronousEvent (AsynchronousEvent const&) = delete;
        AsynchronousEvent (AsynchronousEvent&&)      = delete;
        ~AsynchronousEvent()                         = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the event has been completed yet
         * \return True if the event has already been completed, false otherwise
         */
        RkBool Completed() const noexcept;

        #pragma endregion

        #pragma region Operators

        AsynchronousEvent& operator=(AsynchronousEvent const&) = delete;
        AsynchronousEvent& operator=(AsynchronousEvent&&)      = delete;

        #pragma endregion
};

#include "Core/ExecutiveSystem/AsynchronousEvent.inl"

END_RUKEN_NAMESPACE