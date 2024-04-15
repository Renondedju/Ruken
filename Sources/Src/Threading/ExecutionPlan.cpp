
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
    for (RkSize index = 0ULL; index < m_packs.size(); ++index)
        latches[index] = new std::latch(static_cast<ptrdiff_t>(m_packs[index]));

    // Creating the jobs
    RkSize instruction_index = 0ULL;

    for(RkSize pack_index = 0ULL; pack_index < m_packs.size(); ++pack_index)
    {
        RkSize const start_instruction_index {instruction_index};

        for(;instruction_index < start_instruction_index + m_packs[pack_index]; ++instruction_index)
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

    for(std::latch const* latch: latches)
        delete latch;
}

RkVoid ExecutionPlan::ExecutePlanSynchronously() const noexcept
{
    // Synchronous execution of the plan
    for (auto&& instruction: m_instructions)
        instruction();
}
