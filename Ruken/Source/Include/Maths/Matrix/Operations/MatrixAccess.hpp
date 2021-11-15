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
 * \brief Implements matrix access methods
 */
struct MatrixAccess
{
    #pragma region Static Methods

    /**
     * \brief Getter
     * \param in_matrix Matrix instance
     * \param in_row Row index [0 to TRows)
     * \param in_column Column index [0 to TColumns)
     * \tparam TRows Height or number of rows of the matrix
     * \tparam TColumns Width or number of columns of the matrix
     * \return Constant reference tp the value at the selected position
     */
    template <RkSize TRows, RkSize TColumns>
    [[nodiscard]]
    static constexpr RkFloat const& At(Matrix<TRows, TColumns> const& in_matrix, RkSize const in_row, RkSize const in_column) noexcept
    {
        return in_matrix.data[TColumns * in_row + in_column];
    }

    /**
     * \brief Getter/Setter
     * \param in_matrix Matrix instance
     * \param in_row Row index [0 to TRows)
     * \param in_column Column index [0 to TColumns)
     * \tparam TRows Height or number of rows of the matrix
     * \tparam TColumns Width or number of columns of the matrix
     * \return Reference to the value at the selected position
     */
    template <RkSize TRows, RkSize TColumns>
    [[nodiscard]]
    static constexpr RkFloat& At(Matrix<TRows, TColumns>& in_matrix, RkSize const in_row, RkSize const in_column) noexcept
    {
        return in_matrix.data[TColumns * in_row + in_column];
    }

    #pragma endregion

    /**
     * \brief Implements matrix member access methods
     */
    template <RkSize TRows, RkSize TColumns>
    struct Member
    {
        using TMatrix = Matrix<TRows, TColumns>;

        #pragma region Methods

        /**
         * \brief Getter
         * \param in_row Row index [0 to TRows)
         * \param in_column Column index [0 to TColumns)
         * \return Constant reference tp the value at the selected position
         */
        [[nodiscard]]
        constexpr RkFloat const& At(RkSize const in_row, RkSize const in_column) const noexcept
        {
            return MatrixAccess::At(*static_cast<TMatrix const*>(this), in_row, in_column);
        }

        /**
         * \brief Getter/Setter
         * \param in_row Row index [0 to TRows)
         * \param in_column Column index [0 to TColumns)
         * \return Reference to the value at the selected position
         */
        [[nodiscard]]
        constexpr RkFloat& At(RkSize const in_row, RkSize const in_column) noexcept
        {
            return MatrixAccess::At(*static_cast<TMatrix*>(this), in_row, in_column);
        }

        #pragma endregion
    };
};

END_RUKEN_NAMESPACE