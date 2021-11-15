
#pragma once

#include <vector>

#include "Threading/Job.hpp"
#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Scheduler;

class ExecutionPlan
{
    private:

        #pragma region Members

        // Plan status, only used when constructing the plan
        RkSize m_current_pack_size {0ULL};

        // Execution instructions (aka. the actual update plan)
        std::vector<RkSize> m_packs        {};
        std::vector<Job>    m_instructions {};

        #pragma endregion

    public:

        #pragma region Constructors

        ExecutionPlan()                             = default;
        ExecutionPlan(ExecutionPlan const& in_copy) = default;
        ExecutionPlan(ExecutionPlan&&      in_move) = default;
        ~ExecutionPlan()                            = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Empties the plan and unlocks it so it can be rebuilt
         */
        RkVoid ResetPlan() noexcept;

        /**
         * \brief Adds an instruction to the plan (in the current instruction pack)
         * \param in_instruction Job
         */
        RkVoid AddInstruction(Job&& in_instruction) noexcept;

        /**
         * \brief Ends the current instruction pack, effectively creating a
         *        synchronization barrier before executing the next instruction pack
         */
        RkVoid EndInstructionPack() noexcept;

        /**
         * \brief Starts an asynchronous execution of the plan
         * \param in_scheduler Scheduler
         */
        RkVoid ExecutePlanAsynchronously(Scheduler& in_scheduler) const noexcept;

        /**
         * \brief Starts a synchronous execution of the plan
         */
        RkVoid ExecutePlanSynchronously() const noexcept;

        #pragma endregion

        #pragma region Operators

        ExecutionPlan& operator=(ExecutionPlan const& in_copy) = default;
        ExecutionPlan& operator=(ExecutionPlan&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE