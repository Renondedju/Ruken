#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept NonVoid = !std::is_void_v<TType>;

END_RUKEN_NAMESPACE