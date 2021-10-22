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

#include <array>

#include "Maths/Matrix/GenericMatrix.hpp"

BEGIN_RUKEN_NAMESPACE

template<RkSize TSize>
class __declspec(novtable) SquareMatrix : public GenericMatrix<TSize, TSize>
{
    public:

        using Parent = GenericMatrix<TSize, TSize>;

        #pragma region Constructors

        using GenericMatrix<TSize, TSize>::GenericMatrix;

        constexpr SquareMatrix()                              = default;
        constexpr SquareMatrix(SquareMatrix const& in_matrix) = default;
        constexpr SquareMatrix(SquareMatrix&&      in_matrix) = default;
                 ~SquareMatrix()                              = default;

        DECLARE_MATRIX_COMPATIBILITY_COPY_CONSTRUCTOR(SquareMatrix, Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_CONSTRUCTOR(SquareMatrix, Parent)

        #pragma endregion

        #pragma region Static Methods

        /**
         * \brief Creates an identity matrix, also called unit matrix
         * \return Identity matrix
         */
        static constexpr SquareMatrix Identity() noexcept;

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Computes the transposed matrix
         * \return Transposed matrix
         */
        constexpr SquareMatrix& Transpose() noexcept;

        /**
         * \brief Lower–upper (LU) decomposition or factorization factors a matrix as the product of a lower triangular matrix and an upper triangular matrix.
         * 
         * \param out_l_matrix Lower triangular matrix
         * \param out_u_matrix Upper triangular matrix
         */
        constexpr void LUDecomposition(SquareMatrix<TSize>& out_l_matrix, SquareMatrix<TSize>& out_u_matrix) const noexcept;

        /**
         * \brief Solves Ax = b where A is a lower triangular matrix with non-zero diagonal elements.
         * \param in_vector The b operand in Ax = b
         * \return The resultant x vector
         */
        constexpr std::array<RkFloat, TSize> ForwardSubstitution(std::array<RkFloat, TSize> const& in_vector) const noexcept;

        /**
         * \brief Solves Ax = b where A is a upper triangular matrix with non-zero diagonal elements.
         * \param in_vector The b operand in Ax = b
         * \return The resultant x vector
         */
        constexpr std::array<RkFloat, TSize> BackwardSubstitution(std::array<RkFloat, TSize> const& in_vector) const noexcept;

        /**
         * \brief Inverts this matrix and returns the result
         * \return Inverted matrix
         */
        constexpr SquareMatrix<TSize> GetInverted() const noexcept; 

        #pragma endregion 

        #pragma region Operators

        constexpr SquareMatrix& operator=(SquareMatrix const& in_other) noexcept = default;
        constexpr SquareMatrix& operator=(SquareMatrix&&      in_other) noexcept = default;

        DECLARE_MATRIX_COMPATIBILITY_COPY_OPERATOR(SquareMatrix, Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_OPERATOR(SquareMatrix, Parent)

        #pragma endregion
};

#include "Maths/Matrix/SquareMatrix.inl"

END_RUKEN_NAMESPACE