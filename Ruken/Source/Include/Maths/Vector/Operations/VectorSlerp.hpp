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

#include "Maths/Trigonometry.hpp"
#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \tparam TVector Composed vector type, must inherit this class and be an instance of the Vector class
 */
template <typename TVector>
struct VectorSlerp;

/**
 * \brief Implements vector Slerp method
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorSlerp<Vector<TDimensions, TUnderlyingType>>
{
    using TVector = Vector<TDimensions, TUnderlyingType>;
    using Helper  = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Methods

    /**
     * \brief Performs a circular interpolation between this vector (source) and in_vector (destination)
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Circularly interpolated vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (Vector<TDimensions, TUnderlyingType> in_vector) { in_vector.Dot(); } // Requires the implementation of the Dot method
    [[nodiscard]]
    constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Slerp(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector,
        RkFloat                                        const  in_ratio)
    {
        auto    const dot   = static_cast<TVector*>(this)->Dot(in_vector);
        Radians const theta = ArcCos(dot) * in_ratio;

        return *this * Cos(theta) + (*this - in_vector * dot).Normalized() * Sin(theta);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE