#pragma once

#include "Core/Types/FundamentalTypes.hpp"

#include <tuple>

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Finds the index of the first occurrence of TType in TTuple
 * \note If the type isn't found, this will throw a hard compilation error
 * \tparam TType Type occurrence to look for
 * \tparam TTuple Tuple to look into
 */
template <class TType, class TTuple>
struct TupleIndex;

template <class TType, class... TTypes>
struct TupleIndex<TType, std::tuple<TType, TTypes...>>
{
    static constexpr RkSize value = 0;
};

template <class TType, class TFirst, class... TTypes>
struct TupleIndex<TType, std::tuple<TFirst, TTypes...>>
{
    static constexpr RkSize value = 1 + TupleIndex<TType, std::tuple<TTypes...>>::value;
};

END_RUKEN_NAMESPACE