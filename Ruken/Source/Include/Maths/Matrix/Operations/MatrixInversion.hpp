#pragma once

#include <array>

#include "Maths/Matrix/MatrixForward.hpp"

#include "Maths/Matrix/Operations/MatrixAccess.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix inversion methods
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixInversion
{};

/**
 * \brief Implements matrix inversion methods. Requires a square matrix
 * \warning BUG: Not working for now, do not use this
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixInversion<TRows, TColumns, std::enable_if_t<TRows == TColumns>>
{
    #pragma region Static Methods

    /**
     * \brief Inverts the matrix
     * \note Inverted matrix is such that if multiplied by the original would result in identity matrix
     * \return Inverted matrix
     */
    constexpr Matrix<TRows, TColumns> Inverted() const noexcept
    {
        Matrix<TRows, TColumns> inverted_matrix {};
		Matrix<TRows, TColumns> constexpr identity_matrix {};

		// Computing the LU matrices
		Matrix<TRows, TColumns> lower_matrix;
		Matrix<TRows, TColumns> upper_matrix;

        LuDecomposition(*static_cast<Matrix<TRows, TColumns> const*>(this), lower_matrix, upper_matrix);

        for (RkSize column = 0ULL; column < TRows; ++column)
        {
            std::array<RkFloat, TRows> vector = BackwardSubstitution(upper_matrix,
                ForwardSubstitution(lower_matrix, &identity_matrix.data[column * TRows]).data()
            );

            memcpy(&inverted_matrix.data[column * TRows], vector.data(), sizeof vector);
        }

		return inverted_matrix;
	}

    /**
     * \brief Lower–upper (LU) decomposition or factorization factors a matrix as the product of a lower triangular matrix and an upper triangular matrix.
     * \param in_matrix Matrix instance
     * \param out_lower_matrix Lower (L) matrix instance output
     * \param out_upper_matrix Upper (U) matrix instance output
     */
    static constexpr RkVoid LuDecomposition(Matrix<TRows, TColumns> const& in_matrix,
                                            Matrix<TRows, TColumns>&       out_lower_matrix,
                                            Matrix<TRows, TColumns>&       out_upper_matrix) noexcept
    {
        // Decomposing matrix into Upper and Lower
		// Triangular matrix 
		for (RkSize i {0ULL}; i < TRows; ++i)
		{
			// Upper Triangular 
			for (RkSize k {i}; k < TRows; ++k)
			{
				RkFloat sum {.0F}; 
				for (RkSize j {0ULL}; j < i; ++j)
					sum += MatrixAccess::At(out_lower_matrix, i, j) * MatrixAccess::At(out_upper_matrix, j, k); 

				// Evaluating U(i, k)
				MatrixAccess::At(out_upper_matrix, i, k) = MatrixAccess::At(in_matrix, i, k) - sum;
			}

			// Lower Triangular 
			for (RkSize k {i}; k < TRows; ++k)
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
    static constexpr std::array<RkFloat, TRows> ForwardSubstitution(
        Matrix<TRows, TColumns> const& in_matrix,
        RkFloat const                  in_vector[TRows]) noexcept
    {
        std::array<RkFloat, TRows> output;

        RkFloat sum;
        for(RkSize row {0ULL}; row < TRows; ++row)
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
    static constexpr std::array<RkFloat, TRows> BackwardSubstitution(
        Matrix<TRows, TColumns>    const& in_matrix,
        RkFloat const                     in_vector[TRows]) noexcept
    {
        std::array<RkFloat, TRows> output;

        output[TRows-1] = in_vector[TRows-1] / MatrixAccess::At(in_matrix, TRows-1, TRows-1);

        for (int64_t row = TRows-2; row >= 0; --row)
        {
            output[row] = in_vector[row];
            for (RkSize column = row + 1; column < TRows; ++column)
                output[row] -= MatrixAccess::At(in_matrix, column, row) * output[column];

            output[row] = output[row] / MatrixAccess::At(in_matrix, row, row);
        }

        return output;
    }

	#pragma endregion
};

END_RUKEN_NAMESPACE