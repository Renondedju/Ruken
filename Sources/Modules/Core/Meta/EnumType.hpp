#pragma once

#include "Build/Namespace.hpp"

#include <type_traits>

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept IsEnum = std::is_enum_v<TType>;

END_RUKEN_NAMESPACE