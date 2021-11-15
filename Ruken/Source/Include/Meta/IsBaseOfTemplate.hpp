
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <template <typename...> class TBase, typename TDerived>
struct IsBaseOfTemplateImpl
{
    template<typename... TTypes>
    static constexpr std::true_type  Test(const TBase<TTypes...>*) { return {}; }
    static constexpr std::false_type Test(...)                     { return {}; }

    using Type = decltype(Test(std::declval<TDerived*>()));
};

template <template <typename...> class TBase, typename TDerived>
using IsBaseOfTemplate = typename IsBaseOfTemplateImpl<TBase, TDerived>::Type;

END_RUKEN_NAMESPACE