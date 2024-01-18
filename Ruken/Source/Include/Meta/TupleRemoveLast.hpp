
#pragma once

#include <tuple>

template<class TTuple>
class TupleRemoveLast;

template<class... TArgs>
class TupleRemoveLast<std::tuple<TArgs...>>
{
    using Tuple = std::tuple<TArgs...>;

    template<std::size_t... TIndices>
    static std::tuple<std::tuple_element_t<TIndices, Tuple>...> Extract(std::index_sequence<TIndices...>);

    public:

        using Type = decltype(Extract(std::make_index_sequence<sizeof...(TArgs) - 1>()));
};