
#pragma once

#include "Build/Namespace.hpp"
#include "Meta/IsBaseOfTemplate.hpp"
#include "ECS/Safety/ComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentType... TComponents>
class System;

/**
 * \brief Checks if the passed type is a system
 * \tparam TType Type to check
 */
template <typename TType>
struct IsSystem
{
    static constexpr RkBool value = IsBaseOfTemplate<System, std::remove_const_t<TType>>::value;
};

template <typename TType>
concept SystemType = IsSystem<TType>::value;

END_RUKEN_NAMESPACE