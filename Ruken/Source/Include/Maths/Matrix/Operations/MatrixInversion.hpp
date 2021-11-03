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

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Matrix/Operations/MatrixAccess.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix inversion methods 
 */
struct MatrixInversion
{
    #pragma region Methods

    /**
     * \brief Inverts the passed matrix
     * \note Inverted matrix is such that if multiplied by the original would result in identity matrix
     * \tparam TSize Size of the square matrix to be inverted
     * \param in_matrix Matrix instance
     * \return Inverted matrix
     */
    template <RkSize TSize>
    static constexpr Matrix<TSize, TSize> Invert(Matrix<TSize, TSize> const& in_matrix) noexcept
    {
        Matrix<TSize, TSize> inverted_matrix {};
		Matrix<TSize, TSize> constexpr identity_matrix {};

		// Computing the LU matrices
		Matrix<TSize, TSize> lower_matrix;
		Matrix<TSize, TSize> upper_matrix;

        LuDecomposition(in_matrix, lower_matrix, upper_matrix);

        for (RkSize column = 0ULL; column < TSize; ++column)
        {
            std::array<RkFloat, TSize> vector = BackwardSubstitution(upper_matrix,
                ForwardSubstitution(lower_matrix,
                    std::array<RkFloat, TSize>(&identity_matrix.data[column * TSize])
                )
            );

            memcpy(&inverted_matrix.data[column * TSize], vector.data, sizeof vector);
        }

		return inverted_matrix;
	}

    /**
     * \brief Lower–upper (LU) decomposition or factorization factors a matrix as the product of a lower triangular matrix and an upper triangular matrix.
     * \tparam TSize Size of the matrix
     * \param in_matrix Matrix instance
     * \param out_lower_matrix Lower (L) matrix instance output
     * \param out_upper_matrix Upper (U) matrix instance output
     */
    template <RkSize TSize>
    static constexpr RkVoid LuDecomposition(Matrix<TSize, TSize> const& in_matrix,
                                            Matrix<TSize, TSize>&       out_lower_matrix,
                                            Matrix<TSize, TSize>&       out_upper_matrix) noexcept
    {
        // Decomposing matrix into Upper and Lower
		// Triangular matrix 
		for (RkSize i {0ULL}; i < TSize; ++i)
		{
			// Upper Triangular 
			for (RkSize k {i}; k < TSize; ++k)
			{
				RkFloat sum {.0F}; 
				for (RkSize j {0ULL}; j < i; ++j)
					sum += MatrixAccess::At(out_lower_matrix, i, j) * MatrixAccess::At(out_upper_matrix, j, k); 

				// Evaluating U(i, k)
				MatrixAccess::At(out_upper_matrix, i, k) = MatrixAccess::At(in_matrix, i, k) - sum;
			}

			// Lower Triangular 
			for (RkSize k {i}; k < TSize; ++k)
			{
				if (i == k) 
					MatrixAccess::At(out_lower_matrix, i, i) = 1.0F; // Diagonal as 1 
				else
				{ 
					RkFloat sum = .0F; 
					for (RkSize j = 0; j < i; j++) 
						sum += MatrixAccess::At(out_lower_matrix, k, j) * MatrixAccess::At(out_upper_matrix, j, i); 

					// Evaluating L(k, i) 
					MatrixAccess::At(out_lower_matrix, k, i) = (MatrixAccess::At(in_matrix, k, i) - sum) / MatrixAccess::At(out_upper_matrix, i, i); 
				} 
			}
		}	
    }

    /**
     * \brief Solves Ax = b where A is a lower triangular matrix with non-zero diagonal elements.
     * \param in_matrix The A operand 
     * \param in_vector The b operand
     * \return The resultant x vector
     */
    template <RkSize TSize>
    static constexpr std::array<RkFloat, TSize> ForwardSubstitution(
        Matrix<TSize, TSize>       const& in_matrix,
        std::array<RkFloat, TSize> const& in_vector) noexcept
    {
        std::array<RkFloat, TSize> output;

        RkFloat sum;
        for(RkSize row {0ULL}; row < TSize; ++row)
        {
            sum = 0;
            for(RkSize column = 0; column < row; ++column)
                sum = sum + MatrixAccess::At(in_matrix, row, column) * output[column];

            output[row] = (in_vector[row] - sum) / MatrixAccess::At(in_matrix, row, row);
        }

        return output;
    }

    /**
     * \brief Solves Ax = b where A is a upper triangular matrix with non-zero diagonal elements.
     * \param in_matrix The A operand
     * \param in_vector The b operand
     * \return The resultant x vector
     */
    template <RkSize TSize>
    static constexpr std::array<RkFloat, TSize> BackwardSubstitution(
        Matrix<TSize, TSize>       const& in_matrix,
        std::array<RkFloat, TSize> const& in_vector) noexcept
    {
        std::array<RkFloat, TSize> output;

        output[TSize-1] = in_vector[TSize-1] / MatrixAccess::At(in_matrix, TSize-1, TSize-1);

        for (int64_t row = TSize-2; row >= 0; --row)
        {
            output[row] = in_vector[row];
            for (RkSize column = row + 1; column < TSize; ++column)
                output[row] -= MatrixAccess::At(in_matrix, column, row) * output[column];

            output[row] = output[row] / MatrixAccess::At(in_matrix, row, row);
        }

        return output;
    }

	#pragma endregion

    /**
     * \brief Implements member inversion methods
     * \tparam TSize Size of the square matrix to be inverted
     */
    template <RkSize TSize>
    struct Member
    {
        /**
         * \brief Inverts the matrix
         * \note Inverted matrix is such that if multiplied by the original would result in identity matrix
         * \return Matrix instance
         */
        constexpr Matrix<TSize, TSize>& Invert() noexcept
        {
            *static_cast<Matrix<TSize, TSize>*>(this) = MatrixInversion::Invert(*static_cast<Matrix<TSize, TSize>*>(this));

            return *static_cast<Matrix<TSize, TSize>*>(this);
        }
    };
};

END_RUKEN_NAMESPACE