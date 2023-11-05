#pragma once

#include "Core/ExecutiveSystem/Awaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept AwaitableType = requires(TType in_awaitable)
{
    static_cast<Awaitable<typename std::decay_t<TType>::ProcessingUnit,
                          typename std::decay_t<TType>::Result>>(in_awaitable);
};

END_RUKEN_NAMESPACE