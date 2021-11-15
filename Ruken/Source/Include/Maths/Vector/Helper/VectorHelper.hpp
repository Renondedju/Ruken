
#pragma once

#include <algorithm>

#include "Meta/CommonTypeFallback.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements a set of helper type definitions and methods
 * \tparam TDimensions Dimensions or size of the first vector type
 * \tparam TUnderlyingType Underlying type of the first vector type
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorHelper
{
    #pragma region Type definitions

    /**
     * \brief Returns the largest common vector type
     * \tparam TOtherDimensions Dimensions or size of the other vector
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    using LargestVector = Vector<
        std::max(TDimensions, TOtherDimensions),
        CommonTypeFallback<TUnderlyingType, TUnderlyingType, TOtherUnderlyingType>
    >;

    /**
     * \brief Returns the smallest common vector type
     * \tparam TOtherDimensions Dimensions or size of the other vector
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    using SmallestVector = Vector<
        std::min(TDimensions, TOtherDimensions),
        CommonTypeFallback<TUnderlyingType, TUnderlyingType, TOtherUnderlyingType>
    >;

    /**
     * \brief Returns a common vector type for 2 vectors of the same size 
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     */
    template <typename TOtherUnderlyingType>
    using CommonSizedVector = Vector<
        TDimensions,
        CommonTypeFallback<TUnderlyingType, TUnderlyingType, TOtherUnderlyingType>
    >;

    #pragma endregion
};

END_RUKEN_NAMESPACE