#pragma once

#include <string>

BEGIN_RUKEN_NAMESPACE

class JobQueue;

/**
 * Globally accessible worker info.
 * This interface is accessible via any thread (even non-worker ones) and allows
 * the task system to seamlessly work everywhere in the program.
 */
struct WorkerInfo
{
    std::string name          {"Unnamed worker"};
    JobQueue*   current_queue {nullptr};
    RkUint64    queue_bias    {0};
};

END_RUKEN_NAMESPACE