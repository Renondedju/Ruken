
#pragma once

#include "Build/Namespace.hpp"

#include <mutex>
#include <queue>
#include <atomic>

#include "Types/FundamentalTypes.hpp"
#include "Threading/Synchronized.hpp"
#include "Threading/SynchronizedAccess.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Thread safe locking queue
 * \tparam TType Type contained into the queue
 */
template <typename TType>
class ThreadSafeLockQueue
{
    private:

        #pragma region Memebers

        std::condition_variable         m_empty_notification;
        std::condition_variable         m_push_notification;
        std::atomic_bool                m_unlock_all;
        std::mutex                      m_empty_mutex;
        std::mutex                      m_push_mutex;
        Synchronized<std::queue<TType>> m_queue;
    
        #pragma endregion 

        using QueueReadAccess  = typename decltype(m_queue)::ReadAccess;
        using QueueWriteAccess = typename decltype(m_queue)::WriteAccess;

    public:

        #pragma region Constructors

        ThreadSafeLockQueue();
        ThreadSafeLockQueue(ThreadSafeLockQueue const& in_copy)        = default;
        ThreadSafeLockQueue(ThreadSafeLockQueue&& in_move) noexcept = default;
        ~ThreadSafeLockQueue();

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Releases the queue
         */
        RkVoid Release();

        /**
         * \brief Clears the queue
         */
        RkVoid Clear() noexcept;

        /**
         * \brief Blocks the current thread until the queue gets emptied.
         */
        RkVoid WaitUntilEmpty();

        /**
         * \brief Checks if the queue is empty
         * \return True if the queue is empty, false otherwise
         */
        RkBool Empty() noexcept;

        /**
         * \brief Enqueue an item
         * \param in_item Item to enqueue
         */
        RkVoid Enqueue(TType&& in_item) noexcept;

        /**
         * \brief Tries to dequeue an item, if the queue is empty,
         * \brief the caller thread will be locked until the queue gets a new item or gets released
         * 
         * \param out_item Dequeued item
         * \return True if the content of out_item is valid, false otherwise.
         * 
         * \note This method only returns false if Release() is called from another thread
         */
        RkBool Dequeue(TType& out_item) noexcept;

        #pragma endregion 

        #pragma region Operators

        ThreadSafeLockQueue& operator=(ThreadSafeLockQueue const& in_copy)        = default;
        ThreadSafeLockQueue& operator=(ThreadSafeLockQueue&& in_move) noexcept    = default;

        #pragma endregion 
};

#include "Threading/ThreadSafeLockQueue.inl"

END_RUKEN_NAMESPACE