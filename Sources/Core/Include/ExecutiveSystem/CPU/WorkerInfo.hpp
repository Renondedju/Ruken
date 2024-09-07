#pragma once

#include "ExecutiveSystem/CPU/Queues/CPUQueue.hpp"

#include <string>

BEGIN_RUKEN_NAMESPACE

/**
 * Globally accessible worker info.
 * This interface is accessible via any thread (even non-worker ones) and allows
 * the task system to seamlessly work everywhere in the program.
 */
struct WorkerInfo
{
    #pragma region Members

    inline static thread_local std::string             name            {"Unnamed worker"};
    inline static thread_local CPUQueue* current_queue   {nullptr};
	inline static thread_local RkSize				   remaining_tasks {0};

    #pragma endregion
};

END_RUKEN_NAMESPACE