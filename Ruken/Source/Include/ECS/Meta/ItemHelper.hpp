
#pragma once

#include <tuple>

#include "Build/Namespace.hpp"

#include "Meta/TupleIndex.hpp"
#include "Meta/TupleHasType.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename... TItems>
struct ItemHelper
{
    /**
     * \brief Checks if an item exists in the passed items of the class
     * \tparam TItem Item to look for
     */
    template <typename TItem>
    using ItemExists = TupleHasType<std::remove_const_t<TItem>, std::tuple<std::remove_const_t<TItems>...>>;

    /**
     * \brief Returns the index of an item from the passed items in the class
     * \tparam TField Item to look for
     */
    template <typename TField> requires ItemExists<TField>::value
    using ItemIndex = TupleIndex<std::remove_const_t<TField>, std::tuple<std::remove_const_t<TItems>...>>;
};


END_RUKEN_NAMESPACE