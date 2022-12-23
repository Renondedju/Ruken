#pragma once

#include <coroutine>
#include <atomic_queue/atomic_queue.h>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Concepts/QueueHandleType.hpp"
#include "Core/ExecutiveSystem/ProcessingQueue.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/ConcurrencyCounter.hpp"

BEGIN_RUKEN_NAMESPACE

class Worker;

template <QueueHandleType TQueueHandle>
struct CPUTaskSubscription;

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())

/**
 * \brief Lock-free multi-producer/multi-consumer FIFO queue.
 */
class CentralProcessingQueue: public ProcessingQueue<CentralProcessingUnit>
{
    friend const Worker; // readonly
    friend CPUTaskSubscription; // Updating m_current_concurrency

    #pragma region Members

    std         ::atomic       <RkUint64>                m_concurrency {};
    atomic_queue::AtomicQueueB2<std::coroutine_handle<>> m_queue;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief A simple utility function that attempts to dequeue a job and run it
     *
     * \param out_new_concurrency The new concurrency of the queue. This value is only modified if a job has been ran.
     * \param in_max_attempts Maximum amount of times the operation can be attempted before returning.
     *        A value of 0 will do nothing.
     */
    inline RkVoid TryConsumeJob(ConcurrencyCounter& out_new_concurrency, RkUint32 in_max_attempts) noexcept;

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
		explicit CentralProcessingQueue(RkSize in_size) noexcept;

        CentralProcessingQueue(CentralProcessingQueue const&) = delete;
        CentralProcessingQueue(CentralProcessingQueue&&)      = delete;
        ~CentralProcessingQueue()                             = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Blocking push, waits for available space in the queue 
         * \param in_handle Job handle to push
         */
        RkVoid Push(std::coroutine_handle<>&& in_handle) noexcept;

        /**
         * \brief Attempts to consume jobs of the queue 
         * \param in_sticky When set to true the queue will continue
         *        to consume jobs until the queue no longer requires this much concurrency.
         * \param in_stop_token Stop token. Only useful when in_sticky is true to preemptively stop the loop.
         */
        RkVoid PopAndRun(RkBool in_sticky, std::stop_token const& in_stop_token) noexcept;

        /**
         * \brief Returns the concurrency counter of the queue. This value cannot be used for any kind of synchronization.
         * \return Concurrency counter
         */
        [[nodiscard]]
        ConcurrencyCounter GetConcurrencyCounter() const noexcept;

        // TODO constrained version
        [[nodiscard]]
        RkFloat ComputeOptimalConcurrency(RkUint32 in_max_concurrency) const noexcept;

        #pragma endregion

        #pragma region Operators

        CentralProcessingQueue& operator=(CentralProcessingQueue const&) = delete;
        CentralProcessingQueue& operator=(CentralProcessingQueue&&)      = delete;

        #pragma endregion
};

/**
 * \brief Central processing queue helper
 * This class makes a default constructor available with no arguments required.
 * This is needed by the QueueHandle class.
 *
 * \tparam TSize Size of the queue
 */
template <RkSize TSize>
struct MakeCentralProcessingQueue: CentralProcessingQueue
{ MakeCentralProcessingQueue() noexcept: CentralProcessingQueue {TSize} {} };

#pragma warning(pop)

END_RUKEN_NAMESPACE