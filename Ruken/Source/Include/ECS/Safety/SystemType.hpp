
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

class System;

/**
 * \brief Checks if the passed type is a system
 * \tparam TType Type to check
 */
template <typename TType>
struct IsSystem
{
    static constexpr RkBool value = std::is_base_of_v<System, std::remove_const_t<TType>>;
};

template <typename TType>
concept SystemType = IsSystem<TType>::value;

END_RUKEN_NAMESPACE