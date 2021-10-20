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
 * \brief Implements vector Min and Max methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 * \tparam TVector Composed vector type, must inherit this class and be an instance of the Vector class
 */
template <RkSize TDimensions, typename TUnderlyingType, typename TVector>
requires IsInstance<TVector, Vector>::value // TVector must be a vector type
struct VectorMinMax
{
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Methods

    /**
     * \brief Returns a vector that is made from the largest components of two vectors
     * If one of the vectors is largest than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Max vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Max(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        auto const& smallest {Helper::GetSmallestVector(*this, in_vector)};
        auto        vector   {Helper::GetLargestVector (*this, in_vector)};

        for(RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)
            vector.data[index] = std::max(vector.data[index], smallest.data[index]);

        return vector;
    }

    /**
     * \brief Returns a vector that is made from the smallest components of two vectors
     * If one of the vectors is largest than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Min vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Min(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        auto const& smallest {Helper::GetSmallestVector(*this, in_vector)};
        auto        vector   {Helper::GetLargestVector (*this, in_vector)};

        for(RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)
            vector.data[index] = std::min(vector.data[index], smallest.data[index]);

        return vector;
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE