
#pragma once

#include "Build/Namespace.hpp"
#include "Meta/IsInstance.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentFieldType... TFields>
class SparseComponent;

/**
 * \brief Checks if the passed type is a sparse component
 * \tparam TType Type to check
 */
template <typename TType>
struct IsSparseComponent
{
    static constexpr RkBool value = IsInstance<std::remove_const_t<TType>, SparseComponent>::value;
};

template <typename TType>
concept SparseComponentType = IsSparseComponent<TType>::value;

END_RUKEN_NAMESPACE