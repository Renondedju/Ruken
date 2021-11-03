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
#include "Maths/Matrix/Operations/MatrixAccess.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix multiplication
 * \tparam TRows Height or number of rows of the matrix
 * \tparam TColumns Width or number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixMultiplication
{
    using TMatrix = Matrix<TRows, TColumns>;

    #pragma region Operators

    /**
     * \brief Multiplies two matrices 
     * \tparam TOtherRows Height or number of rows of the other matrix
     * \tparam TOtherColumns Width or number of columns of the other matrix
     * \param in_matrix Right hand side operand
     * \return Multiplied matrix
     */
    template <RkSize TOtherRows, RkSize TOtherColumns>
    requires (TColumns == TOtherRows)
    constexpr Matrix<TRows, TOtherColumns> operator*(Matrix<TOtherRows, TOtherColumns> const& in_matrix) const noexcept
    {
        Matrix<TRows, TOtherColumns> new_matrix;

        for (RkSize row    {0ULL}; row    < TRows;         ++row)
        for (RkSize column {0ULL}; column < TOtherColumns; ++column)
        {
            MatrixAccess::At(new_matrix, row, column) = .0F;
            for (RkSize other_row {0ULL}; other_row < TOtherRows; ++other_row)
                MatrixAccess::At(new_matrix, row, column) +=
                    MatrixAccess::At(*static_cast<TMatrix const*>(this), row, other_row) *
                    MatrixAccess::At(in_matrix, other_row, column);
        }

        return new_matrix;
    }

    /**
     * \brief Multiplies this matrix with another and assigns the result into this matrix (A = A * B)
     * \param in_matrix Other matrix to be multiplied with 
     * \return Matrix instance
     */
    constexpr TMatrix& operator*=(TMatrix const& in_matrix) noexcept
    {
        *static_cast<TMatrix*>(this) = *static_cast<TMatrix*>(this) * in_matrix;

        return *static_cast<TMatrix*>(this);
    }


    #pragma endregion
};

END_RUKEN_NAMESPACE