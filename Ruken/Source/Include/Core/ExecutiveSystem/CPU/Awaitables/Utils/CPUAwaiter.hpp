#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/Awaiter.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

struct CPUAwaiter: Awaiter<CentralProcessingUnit>
{
    CPUAwaiter()                  = default;
    CPUAwaiter(CPUAwaiter const&) = default;
    CPUAwaiter(CPUAwaiter&&     ) = default;
    virtual ~CPUAwaiter()         = default;

    CPUAwaiter& operator=(CPUAwaiter const&) = default;
    CPUAwaiter& operator=(CPUAwaiter&&     ) = default;

    /**
     * \brief Called when an awaited event has been completed
     */
    virtual RkVoid OnContinuation() noexcept = 0;
};

END_RUKEN_NAMESPACE