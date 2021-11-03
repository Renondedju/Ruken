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

#include "Maths/Matrix/MatrixForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix transposition methods
 */
struct MatrixTransposition
{
    #pragma region Static Methods

    /**
     * \brief Transposes the matrix
     * \return Transposed matrix
     */
    template <RkSize TRows, RkSize TColumns>
    [[nodiscard]]
    static constexpr Matrix<TColumns, TRows> Transpose(Matrix<TRows, TColumns> const& in_matrix) noexcept
    {
        Matrix<TColumns, TRows> transposed;

        for (RkSize index {0ULL}; index < TColumns * TRows; ++index)
            transposed.data[index] = in_matrix.data[TColumns * TRows - index];

        return transposed;
    }

    #pragma endregion

	/**
     * \brief Implements member matrix transposition methods
     * \tparam TRows Height or number of rows of the matrix
     * \tparam TColumns Width or number of columns of the matrix
     */
    template <RkSize TRows, RkSize TColumns>
    struct Member
    {
        #pragma region Methods

        /**
         * \brief Transposes the matrix
         * \return Transposed matrix
         */
        [[nodiscard]]
        constexpr Matrix<TColumns, TRows>& Transpose() noexcept
        {
             *static_cast<Matrix<TRows, TColumns>*>(this) = MatrixTransposition::Transpose(*static_cast<Matrix<TRows, TColumns>*>(this));

            return *static_cast<Matrix<TRows, TColumns>*>(this);
        }

        #pragma endregion
    };
};

END_RUKEN_NAMESPACE