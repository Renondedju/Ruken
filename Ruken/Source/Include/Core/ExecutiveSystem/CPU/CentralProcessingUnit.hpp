#pragma once

#include <vector>

#include "Core/ExecutiveSystem/CPU/Worker.hpp"
#include "Core/ExecutiveSystem/ProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

class CentralProcessingQueue;

/**
 * \brief Central Processing Unit
 *
 * This class contains the thread pool required to execute the different tasks.
 * Execution is immediate (greedy) and does not require submission.
 */
class CentralProcessingUnit final: public ProcessingUnit<CentralProcessingUnit, EExecutionPolicy::Immediate, EInstructionType::Direct>
{
    friend CentralProcessingQueue;

    #pragma region Members

    std::vector<CentralProcessingQueue*> m_queues  {};
    std::vector<std::unique_ptr<Worker>> m_workers {};

    #pragma endregion

    public:

        #pragma region Constructors

		explicit CentralProcessingUnit() noexcept;

        CentralProcessingUnit(CentralProcessingUnit const&) = delete;
        CentralProcessingUnit(CentralProcessingUnit&&)      = delete;
        ~CentralProcessingUnit()                            = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Registers the passed queue so it can be processed
         * \param in_queue Queue instance
         */
        RkVoid RegisterQueue(CentralProcessingQueue& in_queue) noexcept;

        #pragma endregion

        #pragma region Operators

        CentralProcessingUnit& operator=(CentralProcessingUnit const&) = delete;
        CentralProcessingUnit& operator=(CentralProcessingUnit&&)      = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE