
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Checks of every of the passed types are unique
 * \tparam TTypes Types to check
 */
template <typename... TTypes>
struct UniqueTypes
{
    static constexpr RkBool value = true;
};

template <typename T1, typename T2>
struct UniqueTypes<T1, T2>
{
    static constexpr RkBool value = !std::is_same<T1, T2>::value;
};

template <typename T1, typename T2, typename... TTypes>
struct UniqueTypes<T1, T2, TTypes...>
{
    static constexpr RkBool value = UniqueTypes<T1, T2>::value && UniqueTypes<T1, TTypes...>::value && UniqueTypes<T2, TTypes...>::value;
};

template <typename... TTypes>
concept Unique = UniqueTypes<TTypes...>::value;

END_RUKEN_NAMESPACE
