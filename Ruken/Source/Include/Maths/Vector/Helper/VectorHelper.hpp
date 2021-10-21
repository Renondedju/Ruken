/**
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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