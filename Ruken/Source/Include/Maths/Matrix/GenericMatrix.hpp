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

#include <ostream>
#include <iomanip>

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"
#include "Types/Concepts/ArithmeticType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Generic matrix implementation
 * 
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
class __declspec(novtable) GenericMatrix
{
	public:

        #pragma region Members

        static constexpr RkSize elements = TRows * TColumns;

        RkFloat data[TRows * TColumns] {.0F};

        #pragma endregion

        #pragma region Constructors

        constexpr GenericMatrix()                               = default;
        constexpr GenericMatrix(GenericMatrix const& in_matrix) = default;
        constexpr GenericMatrix(GenericMatrix&&      in_matrix) = default;
                 ~GenericMatrix()                               = default;

        /**
         * \brief Element by element constructor
         * \tparam TValues Initial values types
         * \param in_values Initial values
         */
        template<typename... TValues> requires (sizeof...(TValues) == TRows * TColumns)
        constexpr GenericMatrix(TValues... in_values) noexcept:
            data {static_cast<RkFloat>(in_values)...}
        {}

        /**
         * \brief Array constructor
         * \param in_values array
         */
        constexpr GenericMatrix(RkFloat const (&in_values)[TRows * TColumns]) noexcept;

        #pragma endregion

        #pragma region Methods
    
        /**
         * \brief Computes the transposed matrix
         * \return Transposed matrix
         */
        [[nodiscard]]
        constexpr GenericMatrix<TColumns, TRows> GetTransposed() const noexcept;

        /**
         * \brief Multiplies this matrix with another one and returns the new resultant matrix 
         * 
         * \tparam TOtherRows Other matrix rows count
         * \tparam TOtherColumns Other matrix columns count
         * 
         * \return New matrix resulting of the multiplication of the 2 previous matrices
         */
        template <RkSize TOtherRows, RkSize TOtherColumns> requires (TColumns == TOtherRows)
        [[nodiscard]] constexpr GenericMatrix<TRows, TOtherColumns> GetMultiplied(GenericMatrix<TOtherRows, TOtherColumns> const& in_other_matrix) const noexcept
        {
            GenericMatrix<TRows, TOtherColumns> new_matrix;

            for (RkSize row = 0ULL; row < TRows; ++row)
            {
                for (RkSize column = 0ULL; column < TOtherColumns; ++column)
                {
                    new_matrix.At(row, column) = .0F;
                    for (RkSize other_row = 0ULL; other_row < TOtherRows; ++other_row)
                        new_matrix.At(row, column) += At(row, other_row) * in_other_matrix.At(other_row, column);
                }
            }

            return new_matrix;
        }

        /**
         * \brief Data getter/setter
         * 
         * \param in_row Row index (from 0 to TRows - 1)
         * \param in_column Column index (from 0 to TColumns - 1)
         * 
         * \return Value at the selected position
         */
        [[nodiscard]] constexpr RkFloat const& At(RkSize in_row, RkSize in_column) const noexcept;
        [[nodiscard]] constexpr RkFloat&       At(RkSize in_row, RkSize in_column)       noexcept;

        #pragma endregion

        #pragma region Operators

        constexpr GenericMatrix& operator=(GenericMatrix const& in_other) noexcept = default;
        constexpr GenericMatrix& operator=(GenericMatrix&&		in_other) noexcept = default;

        #pragma endregion
};

#include "Maths/Matrix/GenericMatrix.inl"

/**
 * \brief ostream <<operator overload for the GenericMatrix class
 * 
 * \tparam TRows Number of rows of the matrix to display
 * \tparam TColumns Number of columns of the matrix to display
 * 
 * \param inout_stream Output stream 
 * \param in_matrix Matrix to display
 * 
 * \return Output stream instance
 */
template<RkSize TRows, RkSize TColumns>
std::ostream& operator<<(std::ostream& inout_stream, GenericMatrix<TRows, TColumns> const& in_matrix)
{
    inout_stream << std::setprecision(3) << std::fixed;
    for (size_t row = 0ull; row < TRows; ++row)
    {
        for (size_t column = 0ull; column < TColumns; ++column)
            inout_stream << ' ' << in_matrix.At(row, column);
        inout_stream << '\n';
    }

    return inout_stream;
}

/**
 * \brief Declares a compatibility copy constructor 
 * \param in_this_type class requiring the compatibility
 * \param in_parent_type Parent class
 */
#define DECLARE_MATRIX_COMPATIBILITY_COPY_CONSTRUCTOR(in_this_type, in_parent_type) \
    constexpr in_this_type(in_parent_type const& in_matrix) noexcept \
    { *this = static_cast<in_this_type>(in_matrix); }

/**
 * \brief Declares a compatibility move constructor 
 * \param in_this_type class requiring the compatibility
 * \param in_parent_type Parent class
 */
#define DECLARE_MATRIX_COMPATIBILITY_MOVE_CONSTRUCTOR(in_this_type, in_parent_type) \
    constexpr in_this_type(in_parent_type&& in_matrix) noexcept \
    { *this = std::forward<in_this_type>(static_cast<in_this_type&&>(in_matrix)); }

/**
 * \brief Declares a compatibility copy operator
 * \param in_this_type class requiring the compatibility
 * \param in_parent_type Parent class
 */
#define DECLARE_MATRIX_COMPATIBILITY_COPY_OPERATOR(in_this_type, in_parent_type) \
    constexpr in_this_type& operator=(in_parent_type const& in_matrix) noexcept \
    { *this = static_cast<in_this_type>(in_matrix); return *this; }

/**
 * \brief Declares a compatibility move operator 
 * \param in_this_type class requiring the compatibility
 * \param in_parent_type Parent class
 */
#define DECLARE_MATRIX_COMPATIBILITY_MOVE_OPERATOR(in_this_type, in_parent_type) \
    constexpr in_this_type& operator=(in_parent_type&& in_matrix) noexcept \
    { *this = std::forward<in_this_type>(static_cast<in_this_type&&>(in_matrix)); return *this; }

END_RUKEN_NAMESPACE