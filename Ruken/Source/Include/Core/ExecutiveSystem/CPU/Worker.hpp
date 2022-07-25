#pragma once

#include <thread>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/ICentralProcessingQueue.hpp"

BEGIN_RUKEN_NAMESPACE

class CentralProcessingUnit; 

/**
 * \brief Single thread of execution with its own internal routine
 *
 * Workers are automatically spawned and owned by a scheduler.
 * Each worker has a given affinity with the queues held by the scheduler (with a global priority)
 * Workers can also be configured with multiple modes, to automatically balance between latency and performance
 */
class Worker
{
    using Dequeue = ICentralProcessingQueue::TryDequeueSignature;

	#pragma region Members

    std::atomic<Dequeue> m_dequeue {};
    std::jthread         m_thread  {};

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Main worker's routine
     * \param in_stop_token Stop token, automatically requested by the thread upon destruction
     * \param in_name Name of the worker thread
     */
    RkVoid Routine(std::stop_token&& in_stop_token, std::string&& in_name) const noexcept;

    #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_name Worker name
         */
        Worker(std::string&& in_name) noexcept;

        Worker()              = default;
        Worker(Worker const&) = delete;
        Worker(Worker&&)      = delete;
        ~Worker()             = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Sets the current working queue of the worker
         * \param in_queue Interface instance
         */
        RkVoid SetQueue(ICentralProcessingQueue const& in_queue);

        #pragma endregion

        #pragma region Operators

        Worker& operator=(Worker const&) = delete;
        Worker& operator=(Worker&&)      = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE