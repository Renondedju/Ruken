#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Common interface holding the suspension callback 
 */
struct CPUAwaitableCallback
{
    CPUAwaitableCallback()                            = default;
    CPUAwaitableCallback(CPUAwaitableCallback const&) = default;
    CPUAwaitableCallback(CPUAwaitableCallback&&     ) = default;
    virtual ~CPUAwaitableCallback()                   = default;

    CPUAwaitableCallback& operator=(CPUAwaitableCallback const&) = default;
    CPUAwaitableCallback& operator=(CPUAwaitableCallback&&     ) = default;

    RkVoid operator()() noexcept
    {
        OnSuspensionCompletion();
    }

    virtual RkVoid OnSuspensionCompletion() noexcept = 0;
};

END_RUKEN_NAMESPACE