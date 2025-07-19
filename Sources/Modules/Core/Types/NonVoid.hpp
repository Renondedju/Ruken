#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept NonVoid = !std::is_same_v<TType, RkVoid>;

END_RUKEN_NAMESPACE