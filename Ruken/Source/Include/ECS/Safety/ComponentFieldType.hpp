
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

#include "ECS/ComponentField.hpp"
#include "Meta/IsInstance.hpp"
#include "Meta/IsBaseOfTemplate.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Checks if the passed type is a valid field
 *        The passed type must:
 *        - Be a direct inheritance from the ComponentField class
 *        - Not be a direct instance of the ComponentField class
 *        - Not be volatile
 *
 * \tparam TType Type to check
 */
template <typename TType>
struct IsComponentField
{
    static constexpr RkBool value = IsBaseOfTemplate<ComponentField, std::remove_const_t<TType>>::value &&
        !IsInstance<std::remove_const_t<TType>, ComponentField>::value &&
        !std::is_volatile<std::remove_const_t<TType>>::value;
};

template <typename TType>
concept ComponentFieldType = IsComponentField<TType>::value;

END_RUKEN_NAMESPACE