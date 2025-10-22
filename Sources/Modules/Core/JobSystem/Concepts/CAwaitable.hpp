#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept CAwaitable = requires (TType const& in_type)
{
	in_type.operator co_await();
};

END_RUKEN_NAMESPACE