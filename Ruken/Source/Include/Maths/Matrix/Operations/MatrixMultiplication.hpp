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
            MatrixAccess::At(new_matrix, row, column) = 0.0F;
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