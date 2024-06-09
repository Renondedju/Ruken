
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"
#include "Meta/HasCommonType.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TAlwaysVoid, typename TFallbackType, typename... TTypes>
struct CommonTypeFallbackImpl
{
	using Type = TFallbackType;
};

template <typename TFallbackType, typename... TTypes>
struct CommonTypeFallbackImpl<std::void_t<std::common_type_t<TTypes...>>, TFallbackType, TTypes...>
{
    using Type = std::common_type_t<TTypes...>;
};

/**
 * \brief Returns a common type if it exists, otherwise returns TFallbackType
 * \tparam TFallbackType Fallback or default type
 * \tparam TTypes Types to test
 */
template <typename TFallbackType, typename... TTypes>
using CommonTypeFallback = CommonTypeFallbackImpl<RkVoid, TFallbackType, TTypes...>::Type;

END_RUKEN_NAMESPACE