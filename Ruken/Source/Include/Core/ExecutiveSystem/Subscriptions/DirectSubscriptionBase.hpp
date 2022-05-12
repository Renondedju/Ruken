#pragma once

#include <atomic>
#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingQueueType TQueue>
class AsynchronousEvent;

/**
 * \brief Base awaiter class for awaiting direct instruction type events
 */
struct DirectSubscriptionBase
{
    using Node = std::atomic<DirectSubscriptionBase*>;

    static constexpr DirectSubscriptionBase* locked    {reinterpret_cast<DirectSubscriptionBase*>(0x1)};
    static constexpr DirectSubscriptionBase* completed {reinterpret_cast<DirectSubscriptionBase*>(0x2)};

    template <ProcessingQueueType TQueue>
    friend class AsynchronousEvent;

    protected:

        #pragma region Members

        /**
         * Represents a thread safe linked list node where AsynchronousEvents
         * contains the head of the list and subscriptions are the actual node containers.
         * Since this node has been designed for co-routines in mind, it can hold some special
         * values listed bellow:
         *
         *  - locked: The next node of the list is currently being modified,
         *  and any iteration of the list must be stopped until the pointer is restored.
         *  - completed: The event that this node was part of has been completed.
         *
         * Any other values simply acts as a classic linked list node pointer.
         */
        Node  m_next  {nullptr};
        Node& m_head; // Reference to the head of the awaited event

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Called when the awaited event has been completed
         */
        virtual RkVoid OnCompletion() noexcept = 0;

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_head Head of the awaited event
         */
        DirectSubscriptionBase(Node& in_head) noexcept;

        DirectSubscriptionBase(DirectSubscriptionBase const&) = delete;
        DirectSubscriptionBase(DirectSubscriptionBase&&)      = delete;
        virtual ~DirectSubscriptionBase() noexcept; 

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the wait has been completed already
         * \note This function can be called even if the awaited event has been deleted already
         * \return True if the awaiter has been completed, false otherwise
         */
        RkBool await_ready() const noexcept;

        /**
         * \brief Attempts a suspension by attaching the awaiter to the awaited event
         * \return True if the suspension succeeded, false otherwise
         */
        RkBool await_suspend(std::coroutine_handle<>) noexcept;

        static constexpr RkVoid await_resume() noexcept {}

        #pragma endregion

        #pragma region Operators

        DirectSubscriptionBase& operator=(DirectSubscriptionBase const&) = delete;
        DirectSubscriptionBase& operator=(DirectSubscriptionBase&&)      = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE