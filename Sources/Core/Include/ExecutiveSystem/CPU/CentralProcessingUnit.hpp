#pragma once

#include <vector>

#include "ExecutiveSystem/CPU/Worker.hpp"
#include "ExecutiveSystem/ProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

class CPUQueue;

/**
 * \brief Central Processing Unit
 *
 * This class contains the thread pool required to execute the different tasks.
 * Execution is immediate (greedy) and does not require submission.
 */
class CentralProcessingUnit final: public ProcessingUnit<CentralProcessingUnit, EExecutionPolicy::Immediate, EInstructionType::Direct>
{
    friend CPUQueue;

    #pragma region Members

    std::vector<CPUQueue*> m_queues  {};
    std::vector<std::unique_ptr<Worker>> m_workers {};

    #pragma endregion

    public:

        #pragma region Constructors

		      explicit CentralProcessingUnit()                    = default;
        CentralProcessingUnit(CentralProcessingUnit const&) = delete;
        CentralProcessingUnit(CentralProcessingUnit&&)      = delete;
        ~CentralProcessingUnit()                            = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Registers the passed queue so it can be processed
         * \param in_queue Queue instance
         */
        RkVoid RegisterQueue(CPUQueue& in_queue) noexcept;

        /**
         * \brief Starts the workers
         */
        RkVoid StartWorkers() noexcept;

        /**
         * \brief Captures the calling thread and uses it as a worker.
         * \note For threads that are not created by the CentralProcessingUnit, make sure to look inside the WorkerInfo struct
         * \param in_should_return Stop token, signals the loop to return as soon as possible
         */
        RkVoid CallerAsWorker(std::stop_token&& in_should_return) const noexcept;

        #pragma endregion

        #pragma region Operators

        CentralProcessingUnit& operator=(CentralProcessingUnit const&) = delete;
        CentralProcessingUnit& operator=(CentralProcessingUnit&&)      = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE