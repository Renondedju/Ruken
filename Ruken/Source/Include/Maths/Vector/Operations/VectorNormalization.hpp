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

#include "Meta/IsInstance.hpp"
#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Normalize and Normalized methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 * \tparam TVector Composed vector type, must inherit this class and be an instance of the Vector class
 */
template <RkSize TDimensions, typename TUnderlyingType, typename TVector>
requires IsInstance<TVector, Vector>::value &&        // TVector must be a vector type
requires (TVector in_vector) { in_vector.Length(); } // And requires the implementation of the Length method
struct VectorNormalization
{
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Methods

    /**
     * \brief Returns the nomalized vector (with a length of 1)
     * If the vector is too small to be normalized a vector zero will be returned instead.
     * \returns Normalized version of the vector 
     */
    [[nodiscard]]
    constexpr TVector Normalized() const noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        TUnderlyingType length = static_cast<TVector*>(this)->Length();
        if (length == TUnderlyingType())
            return {};

        TVector result {*this};
        for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
            result.data[index] /= length;

        return result;
    }

    /**
     * \brief Normalizes the vector (with a length of 1)
     * If the vector is too small to be normalized this method set the vector to 0.
     */
    constexpr RkVoid Normalize() noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        TUnderlyingType length = static_cast<TVector*>(this)->Length();
        if (length == TUnderlyingType())
        {
            for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
                static_cast<TVector*>(this)->data[index] = 0;

            return;
        }

        for(RkSize index {0ULL}; index < TDimensions; ++index)
            static_cast<TVector*>(this)->data[index] /= length;

        return;
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE