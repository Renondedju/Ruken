
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

#include "ECS/Meta/ItemHelper.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ComponentFieldType... TFields>
struct FieldHelper
{
    // Checks if all the passed fields are constant or not
    using Readonly = std::conjunction<std::is_const<TFields>...>;

    /**
     * \brief Checks if a field exists in the passed fields of the class, const modifiers are ignored
     * \tparam TField Field to check for
     */
    template <ComponentFieldType TField>
    using FieldExists = typename ItemHelper<TFields...>::template ItemExists<TField>;

    /**
     * \brief Finds the index of a field, const modifiers are ignored
     * \tparam TField Field to find. This field must exist in the passed fields
     */
    template <ComponentFieldType TField> requires FieldExists<TField>::value
    using FieldIndex = typename ItemHelper<TFields...>::template ItemIndex<TField>;
};

END_RUKEN_NAMESPACE
