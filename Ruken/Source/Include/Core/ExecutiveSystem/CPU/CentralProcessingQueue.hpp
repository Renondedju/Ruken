#pragma once

#include <coroutine>
#include <atomic_queue/atomic_queue.h>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/ProcessingQueue.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())

/**
 * \brief Lock-free multi-producer/multi-consumer FIFO queue.
 * \tparam TInheriting (Curiously Recurring Template Pattern) inheriting class type
 * \tparam TSize Size of the bounded queue (default: 1024 tasks)
 */
template <typename TInheriting, RkSize TSize>
class CentralProcessingQueue: public ProcessingQueue<TInheriting, CentralProcessingUnit>
{
    #pragma region Members

    static inline atomic_queue::AtomicQueue2<std::coroutine_handle<>, TSize> m_queue {};

    #pragma endregion

    public:
        
        #pragma region Constructors

        CentralProcessingQueue()                              = default;
        CentralProcessingQueue(CentralProcessingQueue const&) = delete;
        CentralProcessingQueue(CentralProcessingQueue&&)      = delete;
        ~CentralProcessingQueue()                             = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Blocking push, waits for available space in the queue 
         * \param in_handle Handle to enqueue
         */
        static RkVoid Enqueue(std::coroutine_handle<>&& in_handle) noexcept;

        /**
         * \brief Attempts to pop from the queue if not empty
         * \param out_handle Popped coroutine handle
         * \param in_timeout Number of attempts to be done. Higher values will lead to less latency.
         * \return True if the pop succeeded, false otherwise
         */
        static RkBool TryDequeue(std::coroutine_handle<>& out_handle, RkSize in_timeout) noexcept;

        /**
         * \brief Checks if the queue is empty.
         * \note By the time the value is evaluated, the status of the queue may have changed
         * \return True if the queue was empty, false otherwise
         */
        static RkBool Empty() noexcept;

        #pragma endregion

        #pragma region Operators

        CentralProcessingQueue& operator=(CentralProcessingQueue const&) = delete;
        CentralProcessingQueue& operator=(CentralProcessingQueue&&)      = delete;

        #pragma endregion
};

#pragma warning(pop)

#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.inl"

END_RUKEN_NAMESPACE