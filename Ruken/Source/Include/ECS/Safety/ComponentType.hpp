
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentFieldType... TFields>
class Component;

/**
 * \brief Checks if the passed type is a component
 * \tparam TType Type to check
 */
template <class TType>
struct IsComponent
{
    static constexpr RkBool value = IsBaseOfTemplate<Component, TType>::value;
};

template <typename TType>
concept ComponentType = IsComponent<TType>::value;

END_RUKEN_NAMESPACE