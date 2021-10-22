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
    #pragma region Static Methods

    /**
     * \brief Dot Product of two vectors
     *
     * The dot product is a float value equal to the magnitudes of the
     * two vectors multiplied together and then multiplied by the cosine of the angle between them.
     *
     * \note For normalized vectors Dot returns 1 if they point in exactly the same direction,
     * \note -1 if they point in completely opposite directions and zero if the vectors are perpendicular.
     *
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Left hand side vector
     * \param in_other_vector Right hand side vector
     * \return Dot product
     */
    template<typename TOtherUnderlyingType>
    [[nodiscard]]
    static constexpr RkFloat Dot(Vector<TDimensions, TUnderlyingType>      const& in_vector,
                                 Vector<TDimensions, TOtherUnderlyingType> const& in_other_vector) noexcept
    {
        RkFloat dot {0.0F};

        for (RkSize index = 0; index < TDimensions; index++)
            dot += static_cast<RkFloat>(in_vector.data[index] * in_other_vector.data[index]);

        return dot;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Dot Product of two vectors
     *
     * The dot product is a float value equal to the magnitudes of the
     * two vectors multiplied together and then multiplied by the cosine of the angle between them.
     *
     * \note For normalized vectors Dot returns 1 if they point in exactly the same direction,
     * \note -1 if they point in completely opposite directions and zero if the vectors are perpendicular.
     *
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Right hand side vector
     * \return Dot product
     */
    template<typename TOtherUnderlyingType>
    [[nodiscard]]
    constexpr RkFloat Dot(Vector<TDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        return Dot(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_vector);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE