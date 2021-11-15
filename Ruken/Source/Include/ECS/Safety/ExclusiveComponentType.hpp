
#pragma once

#include "Build/Namespace.hpp"
#include "Meta/IsInstance.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentFieldType... TFields>
class ExclusiveComponent;

/**
 * \brief Checks if the passed component is an exclusive component
 * \tparam TType Type to check
 */
template <typename TType>
struct IsExclusiveComponent
{
    static constexpr RkBool value = IsInstance<std::remove_const_t<TType>, ExclusiveComponent>::value;
};

template <typename TType>
concept ExclusiveComponentType = IsExclusiveComponent<TType>::value;

END_RUKEN_NAMESPACE