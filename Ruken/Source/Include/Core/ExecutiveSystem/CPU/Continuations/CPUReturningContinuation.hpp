#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TReturnType = RkVoid>
struct CPUReturningContinuation: CPUContinuation
{
    using Node = std::atomic<CPUContinuation*>;

    TReturnType& return_value;
};

template <>
struct CPUReturningContinuation<RkVoid>: CPUContinuation
{};

END_RUKEN_NAMESPACE