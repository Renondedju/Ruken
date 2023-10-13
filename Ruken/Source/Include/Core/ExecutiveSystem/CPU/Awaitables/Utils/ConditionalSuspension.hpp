#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct ConditionalSuspension
{
    RkBool condition;

    [[nodiscard]] bool await_ready() const noexcept
    { return condition; }

    static constexpr void await_suspend(std::coroutine_handle<>) noexcept {}
    static constexpr void await_resume ()                        noexcept {}
};

END_RUKEN_NAMESPACE