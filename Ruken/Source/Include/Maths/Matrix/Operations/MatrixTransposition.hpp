
#pragma once

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Matrix/Operations/MatrixAccess.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements member matrix transposition methods
 * \tparam TRows Height or number of rows of the matrix
 * \tparam TColumns Width or number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixTransposition
{
    #pragma region Methods

    /**
     * \brief Transposes the matrix
     * \return Transposed matrix
     */
    [[nodiscard]]
    constexpr Matrix<TColumns, TRows> Transposed() const noexcept
    {
        Matrix<TColumns, TRows> transposed;

        for (RkSize row {0ULL}; row < TRows; ++row)
            for (RkSize column {0ULL}; column < TColumns; ++column)
                MatrixAccess::At(transposed, column, row) = MatrixAccess::At(*static_cast<Matrix<TRows, TColumns> const*>(this), row, column);

        return transposed;
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE