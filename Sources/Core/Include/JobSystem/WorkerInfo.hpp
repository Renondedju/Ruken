#pragma once

#include "JobSystem/Queues/JobQueue.hpp"

#include <string>

BEGIN_RUKEN_NAMESPACE

/**
 * Globally accessible worker info.
 * This interface is accessible via any thread (even non-worker ones) and allows
 * the task system to seamlessly work everywhere in the program.
 */
struct WorkerInfo
{
    inline static thread_local std::string name            {"Unnamed worker"};
    inline static thread_local JobQueue*   current_queue   {nullptr};
	inline static thread_local RkSize	   remaining_tasks {0};
};

END_RUKEN_NAMESPACE