#pragma once

#include "Build/Namespace.hpp"

#include <type_traits>

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept CTaskResult = std::is_destructible_v<TType> || std::is_void_v<TType>;

END_RUKEN_NAMESPACE