
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Returns the minimum type required to store an object containing ``TSize`` bytes
 * \tparam TSize Number of required bytes
 * \tparam TExcessType Excess type, default is RkVoid
 */
template <RkUint8 TSize, typename TExcessType = RkVoid>
struct MinimumType {
    using Type = std::conditional_t<TSize == 0u , RkVoid,
                 std::conditional_t<TSize <= 8u , RkUint8,
                 std::conditional_t<TSize <= 16u, RkUint16,
                 std::conditional_t<TSize <= 32u, RkUint32,
                 std::conditional_t<TSize <= 64u, RkUint64,
        TExcessType>>>>>;
};

/**
 * \brief Helper type alias for MinimumType<TSize>::Type
 * \tparam TSize Number of required bytes
 * \tparam TExcessType Excess type, default is RkVoid
 */
template <RkUint8 TSize, typename TExcessType = RkVoid>
using MinimumTypeT = typename MinimumType<TSize, TExcessType>::Type;

END_RUKEN_NAMESPACE
