
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TAlwaysVoid, typename... TTypes>
struct HasCommonTypeImpl : std::false_type {};

template <typename... TTypes>
struct HasCommonTypeImpl<std::void_t<std::common_type_t<TTypes...>>, TTypes...> : std::true_type {};

/**
 * \brief Checks if a std::common_type exists for the passed types
 * \tparam TTypes Types to check
 */
template <typename... TTypes>
using HasCommonType = typename HasCommonTypeImpl<RkVoid, TTypes...>::type;

END_RUKEN_NAMESPACE