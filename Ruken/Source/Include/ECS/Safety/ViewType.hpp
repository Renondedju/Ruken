
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"
#include "Meta/IsInstance.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentFieldType... TFields>
class ComponentView;

/**
 * \brief Checks if the passed type is a valid view
 *        The passed type must:
 *        - Be a direct instance of the ComponentView class
 *        - Not be volatile
 *
 * \tparam TType Type to check
 */
template <typename TType>
struct IsView
{
    static constexpr RkBool value = IsInstance<std::remove_const_t<TType>, ComponentView>::value && !std::is_volatile_v<std::remove_const_t<TType>>;
};

template <typename TType>
concept ViewType = IsView<TType>::value;

/**
 * \brief Checks if the passed type is a valid view
 *        The passed type must:
 *        - Be a direct instance of the ComponentView class
 *        - Not be volatile
 *        - Be readonly (aka. have every field marked as const)
 *
 * \tparam TType Type to check
 */
template <typename TType>
struct IsReadonlyView
{
    static constexpr RkBool value = IsView<TType>::value && TType::IsReadonly::value;
};

template <typename TType>
concept ReadonlyViewType = IsReadonlyView<TType>::value;

END_RUKEN_NAMESPACE