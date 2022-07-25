#pragma once

#include <string>

#include "Core/ExecutiveSystem/CPU/ICentralProcessingQueue.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Globally accessible worker info.
 * This interface is accessible via any thread (even non-worker ones) and allows
 * the task system to seamlessly work everywhere in the program.
 */
struct WorkerInfo
{
    #pragma region Members

    inline static thread_local std::string              name          {};
    inline static thread_local ICentralProcessingQueue* current_queue {};

    #pragma endregion
};

END_RUKEN_NAMESPACE