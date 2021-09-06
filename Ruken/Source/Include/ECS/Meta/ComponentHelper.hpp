
#pragma once

#include "Build/Namespace.hpp"

#include "ECS/Meta/ItemHelper.hpp"
#include "ECS/Safety/AnyComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TTuple>
struct ComponentHelper;

template <AnyComponentType... TComponents>
struct ComponentHelper<std::tuple<TComponents...>>
{
    /**
     * \brief Checks if a component exists in the passed components of the class, const modifiers are ignored
     * \tparam TComponent Component to check for
     */
    template <AnyComponentType TComponent>
    using ComponentExists = typename ItemHelper<TComponents...>::template ItemExists<TComponent>;

    /**
     * \brief Finds the index of a component, const modifiers are ignored
     * \tparam TComponent Component to find. This component must exist in the passed fields
     */
    template <AnyComponentType TComponent> requires ComponentExists<TComponent>::value
    using ComponentIndex = typename ItemHelper<TComponents...>::template ItemIndex<TComponent>;
};

END_RUKEN_NAMESPACE
