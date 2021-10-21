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

#include "Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \tparam TVector Composed vector type, must inherit this class and be an instance of the Vector class
 */
template <typename TVector>
struct VectorDot;

/**
 * \brief Implements vector Dot method
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorDot<Vector<TDimensions, TUnderlyingType>>
{
    using TVector = Vector<TDimensions, TUnderlyingType>;

    #pragma region Methods

    /**
     * \brief Dot Product of two vectors
     *
     * \note For normalized vectors Dot returns 1 if they point in exactly the same direction,
     * \note -1 if they point in completely opposite directions and zero if the vectors are perpendicular.
     *
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Right hand side vector
     * \return Dot product
     */
    template<typename TOtherUnderlyingType>
    requires requires(TUnderlyingType                                                            in_type,
                      TOtherUnderlyingType                                                       in_other_type,
                      CommonTypeFallback<TUnderlyingType, TUnderlyingType, TOtherUnderlyingType> in_common_type)
    {
        in_common_type += in_type * in_other_type;
    }
    [[nodiscard]]
    constexpr CommonTypeFallback<TUnderlyingType, TUnderlyingType, TOtherUnderlyingType> Dot(
        Vector<TDimensions, TOtherUnderlyingType> const& in_vector)
    {
        CommonTypeFallback<TUnderlyingType, TUnderlyingType, TOtherUnderlyingType> dot {0};

        for (RkSize index = 0; index < TDimensions; index++)
            dot += static_cast<TVector*>(this)->data[index] * in_vector.data[index];

        return dot;
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE