#pragma once

#include "Types/FundamentalTypes.hpp"
#include "ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "ExecutiveSystem/ProcessingQueue.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/ConcurrencyCounter.hpp"

#include <atomic_queue/atomic_queue.h>
#include <condition_variable>
#include <tracy/Tracy.hpp>
#include <coroutine>

BEGIN_RUKEN_NAMESPACE
class Worker;

template <QueueHandleType TQueueHandle>
struct CPUTaskSubscription;

#pragma warning(push)
#pragma warning(disable: 4324)

// Disabled warning 4324: Structure was padded due to __declspec(align())
// This is related to the way atomic values interacts with cache lines and is expected of the atomic_queue

/**
 * \brief Lock-free multi-producer/multi-consumer FIFO queue.
 */
class CPUQueue: public ProcessingQueue<CentralProcessingUnit>
{
    template <QueueHandleType TFriendQueueHandle>
    friend struct CPUTaskSubscription; // Updating m_current_concurrency
    friend Worker; // readonly

    #pragma region Members

    atomic_queue::AtomicQueueB2<std::coroutine_handle<>> m_queue;

    // Tracking
    std::atomic<RkUint64>               m_concurrency        {};
    mutable std::condition_variable_any m_condition_variable {};
    mutable std::mutex                  m_sleep_mutex        {};

    #pragma endregion

    #pragma region Methods

    /**
     * \brief A simple utility function that attempts to dequeue a job and run it.
     * \param in_max_attempts Maximum amount of times the operation can be attempted before returning.
     *        A value of 0 will do nothing.
     */
    inline RkVoid TryConsumeJob(RkUint32 in_max_attempts) noexcept;

    /**
     * \brief Helper function returning the signed concurrency request of the queue.
     *        If this function returns 2.3f, that means 2 full time workers + 30% of a 3rd one are requested.
     *        Negative values indicates a surplus of concurrency.
     *
     * \param in_concurrency Fetched concurrency
     * \param in_offset Current concurrency offset.
     *        This allows to get the concurrency request as if x workers were to be added/removed from the queue.
     *
     * \return Signed concurrency request
     */
    inline RkFloat GetSignedConcurrencyRequest(ConcurrencyCounter const& in_concurrency, RkUint32 in_offset = 0U) const noexcept;

    #pragma endregion

    public:
        
        #pragma region Constructors

        /**
      		 * \brief Default constructor
		       * \param in_size Size of the queue
		       */
		      explicit CPUQueue(RkSize in_size) noexcept;

        CPUQueue(CPUQueue const&) = delete;
        CPUQueue(CPUQueue&&)      = delete;
        ~CPUQueue()               = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Yielding to the queue will sleep the calling
         *        thread until it is needed again by this queue.
         */
        RkVoid Yield(std::stop_token const& in_stop_token) const noexcept;

        /**
         * \brief Blocking push, waits for available space in the queue 
         * \param in_handle Job handle to push
         */
        RkVoid Push(std::coroutine_handle<> in_handle) noexcept;

        /**
         * \brief Attempts to consume jobs of the queue 
         * \param in_greedy When set to true the queue will continue
         *        to consume jobs until the queue no longer requires this much concurrency.
         * \param in_stop_token Stop token. Only useful when in_greedy is true to preemptively stop the loop.
         */
        RkVoid PopAndRun(RkBool in_greedy, std::stop_token const& in_stop_token) noexcept;

        // TODO constrained version
        [[nodiscard]]
        RkFloat ComputeOptimalConcurrency(RkUint32 in_max_concurrency) const noexcept;

        #pragma endregion

        #pragma region Operators

        CPUQueue& operator=(CPUQueue const&) = delete;
        CPUQueue& operator=(CPUQueue&&)      = delete;

        #pragma endregion
};

#pragma warning(pop)

END_RUKEN_NAMESPACE