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

#include <latch>

#include "Threading/Scheduler.hpp"
#include "Threading/ExecutionPlan.hpp"

USING_RUKEN_NAMESPACE

RkVoid ExecutionPlan::ResetPlan() noexcept
{
    m_current_pack_size = 0;

    m_packs       .clear();
    m_instructions.clear();
}

RkVoid ExecutionPlan::AddInstruction(Job&& in_instruction) noexcept
{
    // Adding the new instruction
    m_instructions.emplace_back(std::forward<Job>(in_instruction));

    // Increasing the pack size
    m_current_pack_size++;
}

RkVoid ExecutionPlan::EndInstructionPack() noexcept
{
    // If there is no pack to wrap up, returning
    if (m_current_pack_size == 0ULL)
        return;

    m_packs.emplace_back(m_current_pack_size);
    m_current_pack_size = 0ULL;
}

RkVoid ExecutionPlan::ExecutePlanAsynchronously(Scheduler& in_scheduler) const noexcept
{
    std::vector<std::latch*> latches {m_packs.size()};

    // Creating the latches
    for (RkSize pack: m_packs)
        latches.emplace_back(new std::latch(pack));

    // Creating the jobs
    RkSize instruction_index = 0ULL;

    for(RkSize pack_index = 0ULL; pack_index < m_packs.size(); ++pack_index)
    {
        RkSize const pack_end = instruction_index + m_packs[pack_index];

        for(;instruction_index < pack_end; ++instruction_index)
        {
            // FIXME: Scheduling every job right away
            //
            // This will consume every threads of the thread pool at once if there are more instructions than threads
            // This might be a problem later on since some threads are consumed
            // to wait other instructions, resulting in a potential stall if anything else needs to be executed asynchronously in the same time
            in_scheduler.ScheduleTask([this, &latches, pack_index, instruction_index] {

                // Waiting for the previous instruction pack to be done
                // (If there is one)
                if (pack_index > 0)
                    latches[pack_index - 1]->wait();

                // Executing the current instruction
                m_instructions[instruction_index]();

                // Notifying the pack that one instruction has been done
                // If the pack is completed, this will allow the next pack to start execution
                latches[pack_index]->count_down();
            });
        }
    }

    // Waiting for the last instruction pack to be executed
    // ie. waiting for the plan to be executed
    latches[latches.size() - 1]->wait();

    for(std::latch* latch: latches)
        delete latch;
}

RkVoid ExecutionPlan::ExecutePlanSynchronously() const noexcept
{
    // Synchronous execution of the plan
    for (auto&& instruction: m_instructions)
        instruction();
}
