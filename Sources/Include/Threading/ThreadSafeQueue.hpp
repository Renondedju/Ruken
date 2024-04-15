
#pragma once

#include "Build/Namespace.hpp"

#include "Containers/Queue.hpp"
#include "Threading/Synchronized.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Thread safe queue encapsulation
 * \tparam TType Type contained into the queue
 */
template <typename TType>
class ThreadSafeQueue
{
    private:

        #pragma region Members

        Synchronized<Queue<TType>> m_queue;

        using QueueReadAccess  = decltype(m_queue)::ReadAccess;
        using QueueWriteAccess = decltype(m_queue)::WriteAccess;

        #pragma endregion

    public:

        #pragma region Constructors

        ThreadSafeQueue()                                    = default;
        ThreadSafeQueue(ThreadSafeQueue const& in_copy)        = default;
        ThreadSafeQueue(ThreadSafeQueue&& in_move) noexcept = default;
        ~ThreadSafeQueue()                                    = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the queue is empty
         * \return True if the queue is empty, false otherwise
         */
        RkBool Empty() const noexcept;

        /**
         * \brief Enqueue an item
         * \param in_item Item to enqueue
         */
        RkVoid Enqueue(TType&& in_item) noexcept;

        /**
         * \brief Tries to dequeue an item
         * \warning If the queue is empty, the result of this function and the content of out_item are undefined
         * \param out_item Dequeued item
         */
        RkVoid Dequeue(TType& out_item) noexcept;

        #pragma endregion

        #pragma region Operators

        ThreadSafeQueue& operator=(ThreadSafeQueue const& in_copy)        = default;
        ThreadSafeQueue& operator=(ThreadSafeQueue&& in_move) noexcept    = default;

        #pragma endregion
};

#include "Threading/ThreadSafeQueue.inl"

END_RUKEN_NAMESPACE
