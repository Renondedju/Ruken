#pragma once

#include "Core/ExecutiveSystem/AsynchronousEvent.hpp"
#include "Core/ExecutiveSystem/TaskCompletionEvent.hpp"
#include "Core/ExecutiveSystem/CPU/CPUTaskSubscription.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Central processing unit manual reset event
 */
struct ManualResetEvent: AsynchronousEvent<CentralProcessingUnit>
{
    protected:

        #pragma region Members

        mutable CPUSubscription::Node m_head {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        ManualResetEvent ()                        = default;
        ManualResetEvent (ManualResetEvent const&) = delete;
        ManualResetEvent (ManualResetEvent&&)      = delete;
        ~ManualResetEvent()                        = default;

        #pragma endregion

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

        /**
         * \brief Checks if the event has been completed yet
         * \return True if the event has already been completed, false otherwise
         */
        [[nodiscard]]
		RkBool Completed() const noexcept;

        /**
         * \brief Creates a subscription for a given task
         * \tparam TQueueHandle Handle type to the queue the awaiting task lives in
         * \param in_task Awaiting task instance
         * \return Subscription instance
         */
        template <QueueHandleType TQueueHandle>
        CPUTaskSubscription<TQueueHandle> GetSubscription(Task<TQueueHandle>&& in_task) const noexcept
        {
            return CPUTaskSubscription<TQueueHandle>(m_head, std::forward<Task<TQueueHandle>>(in_task));
        }

        #pragma endregion

        #pragma region Operators

        ManualResetEvent& operator=(ManualResetEvent const&) = delete;
        ManualResetEvent& operator=(ManualResetEvent&&)      = delete;

        #pragma endregion
};

// ManualResetEvent is the default task completion event
template <>
struct TaskCompletionEvent<CentralProcessingUnit>
{ using Type = ManualResetEvent; };

END_RUKEN_NAMESPACE