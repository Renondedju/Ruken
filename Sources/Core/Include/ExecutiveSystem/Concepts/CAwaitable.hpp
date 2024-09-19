#pragma once

#include "ExecutiveSystem/Awaitable.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept CAwaitable = requires(TType in_awaitable)
{
    static_cast<Awaitable<typename std::decay_t<TType>::ProcessingUnit,
                          typename std::decay_t<TType>::Result,
								   std::decay_t<TType>::reliable>
	>(in_awaitable);
};

END_RUKEN_NAMESPACE