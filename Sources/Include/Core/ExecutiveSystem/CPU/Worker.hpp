#pragma once

#include <string>
#include <thread>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class CentralProcessingQueue;

/**
 * \brief Single thread of execution with its own internal routine
 */
class Worker
{
	#pragma region Members

    std::vector<CentralProcessingQueue*>& m_queues;
    std::jthread                          m_thread {};

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
         * \param in_queues Queues to work on
         */
		explicit Worker(std::string in_name, std::vector<CentralProcessingQueue*>& in_queues) noexcept;

        Worker(Worker const&) = delete;
        Worker(Worker&&)      = delete;
        ~Worker()             = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Runs jobs on the passed queues for a maximum of one full cycle.
         * \param in_queues Queues to cycle though 
         * \param in_stop_token If a stop is requested, the method will return as soon as the current job is done
         */
        static RkVoid ProcessQueues(std::vector<CentralProcessingQueue*> const& in_queues, std::stop_token const& in_stop_token) noexcept;

        #pragma endregion

        #pragma region Operators

        Worker& operator=(Worker const&) = delete;
        Worker& operator=(Worker&&)      = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE