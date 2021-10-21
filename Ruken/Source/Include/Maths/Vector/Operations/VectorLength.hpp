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

#include "Maths/Math.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \tparam TVector Composed vector type, must inherit this class and be an instance of the Vector class
 */
template <typename TVector>
struct VectorLength;

/**
 * \brief Implements vector Length and SqrLength methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorLength<Vector<TDimensions, TUnderlyingType>>
{
    using TVector = Vector<TDimensions, TUnderlyingType>;

    #pragma region Methods

    /**
     * \brief Computes the square length of the vector
     * \return Square length of the vector
     */
    [[nodiscard]]
    constexpr TUnderlyingType SquareLength() const noexcept
    requires requires(TUnderlyingType in_a) { in_a += in_a; in_a * in_a; }
    {                 
        TUnderlyingType square_length {};
                                                                                                                                                                                                             
        for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
            square_length += static_cast<TVector const*>(this)->data[index] * static_cast<TVector const*>(this)->data[index];                                                                                                                               
                                                                                                                                                                                                             
        return square_length;                                                                                                                                                                                       
    }

    /**
     * \brief Computes the length of the vector
     * \note If you only need to compare magnitudes of some vectors, you can compare squared magnitudes of
     * \note them using SquareLength instead (computing squared magnitude is faster).
     * \return Length of the vector
     */
    [[nodiscard]]
    constexpr TUnderlyingType Length() const noexcept
    {                 
        return static_cast<TUnderlyingType>(Sqrt(static_cast<RkFloat>(SquareLength())));                                                                                                                                                                                       
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE