
#pragma once

#include <type_traits>
#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TType>
concept FloatingPointType = std::is_floating_point_v<TType>;

END_RUKEN_NAMESPACE