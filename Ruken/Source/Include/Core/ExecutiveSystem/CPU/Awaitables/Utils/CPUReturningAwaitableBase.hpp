#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct CPUReturningAwaitableBase
{
    #pragma region Lifetime

    CPUReturningAwaitableBase()                                 = default;
    CPUReturningAwaitableBase(CPUReturningAwaitableBase const&) = default;
    CPUReturningAwaitableBase(CPUReturningAwaitableBase&&     ) = default;
    virtual         ~CPUReturningAwaitableBase()                = default;

    CPUReturningAwaitableBase& operator=(CPUReturningAwaitableBase const&) = default;
    CPUReturningAwaitableBase& operator=(CPUReturningAwaitableBase&&     ) = default;

    #pragma endregion

    virtual RkVoid NotifyOrphanedAwaitable() noexcept {}
};

END_RUKEN_NAMESPACE