#pragma once

#include <coroutine>
#include <atomic_queue/atomic_queue.h>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/ProcessingQueue.hpp"
#include "Core/ExecutiveSystem/CPU/ECPUQueueGroup.hpp"
#include "Core/ExecutiveSystem/CPU/ECPUPipelinePolicy.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())

/**
 * \brief Lock-free multi-producer/multi-consumer FIFO queue.
 */
class CentralProcessingQueue: public ProcessingQueue<CentralProcessingUnit>
{
    #pragma region Members

    atomic_queue::AtomicQueueB2<std::coroutine_handle<>> m_queue;

    #pragma endregion

    public:
        
        #pragma region Constructors

        /**
		 * \brief Default constructor
		 * \param in_size Size of the queue
		 * \param in_group Queue group
		 * \param in_pipeline_policy Pipeline policy
		 */
		explicit CentralProcessingQueue(RkSize in_size, ECPUQueueGroup in_group, ECPUPipelinePolicy in_pipeline_policy) noexcept;

        CentralProcessingQueue(CentralProcessingQueue const&) = delete;
        CentralProcessingQueue(CentralProcessingQueue&&)      = delete;
        ~CentralProcessingQueue()                             = default;

        #pragma endregion

		#pragma region Members

        const ECPUQueueGroup     group;
        const ECPUPipelinePolicy pipeline_policy;

		#pragma endregion

        #pragma region Methods

        /**
         * \brief Blocking push, waits for available space in the queue 
         * \param in_handle Handle to enqueue
         */
        RkVoid Enqueue(std::coroutine_handle<>&& in_handle) noexcept;

        /**
         * \brief Attempts to pop from the queue if not empty
         * \param out_handle Popped coroutine handle
         * \param in_timeout Number of attempts to be done. Higher values will lead to less latency.
         * \return True if the pop succeeded, false otherwise
         */
        RkBool TryDequeue(std::coroutine_handle<>& out_handle, RkSize in_timeout) noexcept;

        /**
         * \brief Checks if the queue is empty.
         * \note By the time the value is evaluated, the status of the queue may have changed
         * \return True if the queue was empty, false otherwise
         */
		[[nodiscard]]
		RkBool Empty() const noexcept;

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
 * \tparam TGroup Group of the queue
 * \tparam TPolicy Pipeline policy of the queue
 */
template <RkSize TSize, ECPUQueueGroup TGroup, ECPUPipelinePolicy TPolicy>
struct MakeCentralProcessingQueue: CentralProcessingQueue
{ MakeCentralProcessingQueue() noexcept: CentralProcessingQueue {TSize, TGroup, TPolicy} {} };

#pragma warning(pop)

END_RUKEN_NAMESPACE