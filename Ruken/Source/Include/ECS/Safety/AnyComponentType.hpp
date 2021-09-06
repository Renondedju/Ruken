
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

class ComponentBase;

/**
 * \brief Checks if the passed type is a component, of any type
 * \tparam TType Type to check
 */
template <typename TType>
struct IsAnyComponent
{
    static constexpr RkBool value = std::is_base_of<ComponentBase, std::remove_const_t<TType>>::value;
};

template <typename TComponent>
concept AnyComponentType = IsAnyComponent<TComponent>::value;

END_RUKEN_NAMESPACE