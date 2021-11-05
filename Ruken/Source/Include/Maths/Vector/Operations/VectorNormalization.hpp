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

#include "Maths/Vector/Helper/VectorHelper.hpp"
#include "Maths/Vector/Operations/VectorLength.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Normalize and Normalized methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorNormalization
{
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    /**
     * \brief Returns the nomalized vector (with a length of 1)
     * If the vector is too small to be normalized a vector zero will be returned instead.
     *
     * \param in_vector Vector instance
     * \returns Normalized version of the vector 
     */
    [[nodiscard]]
    static constexpr Vector<TDimensions, TUnderlyingType> Normalized(Vector<TDimensions, TUnderlyingType> const& in_vector) noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        TUnderlyingType length = VectorLength<Vector<TDimensions, TUnderlyingType>>::Length(in_vector);
        if (length == TUnderlyingType())
            return {};

        Vector<TDimensions, TUnderlyingType> result {in_vector};
        for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
            result.data[index] /= length;

        return result;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Returns the nomalized vector (with a length of 1)
     * If the vector is too small to be normalized a vector zero will be returned instead.
     * \returns Normalized version of the vector 
     */
    [[nodiscard]]
    constexpr auto Normalized() const noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        return Normalized(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    /**
     * \brief Normalizes the vector (with a length of 1)
     * If the vector is too small to be normalized this method set the vector to 0.
     */
    constexpr Vector<TDimensions, TUnderlyingType>& Normalize() noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        return *static_cast<Vector<TDimensions, TUnderlyingType>*>(this) = Normalized(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE