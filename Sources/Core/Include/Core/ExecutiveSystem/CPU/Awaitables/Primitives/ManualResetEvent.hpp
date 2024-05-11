#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Central processing unit manual reset event
 */
template <typename TResult, RkBool TNoexcept>
struct ManualResetEvent final: CPUAwaitable<TResult, TNoexcept>
{
    protected:

        // RAII controlled
        RkVoid Deallocate() override {}
};

END_RUKEN_NAMESPACE