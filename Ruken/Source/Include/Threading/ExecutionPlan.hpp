/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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