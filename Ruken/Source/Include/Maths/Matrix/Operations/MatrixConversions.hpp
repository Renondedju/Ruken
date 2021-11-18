#pragma once

#include <algorithm>

#include "Maths/Matrix/MatrixForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix utilities
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixConversions
{
    /**
	 * \brief Matrix conversion operator
	 * \note This operator is only explicit if one of the new sizes of the matrix is smaller than the original one 
	 * \tparam TNewRows New height or number of rows of the matrix
	 * \tparam TNewColumns New width or number of columns of the matrix
	 */
	template <RkSize TNewRows, RkSize TNewColumns>
	explicit(TNewRows < TRows || TNewColumns < TColumns)
    operator Matrix<TNewRows, TNewColumns>() const noexcept
	{
		// Will be identity if the matrix is a square matrix
		Matrix<TNewRows, TNewColumns> new_matrix {};

		for (RkSize column {0ULL}; column < std::min(TColumns, TNewColumns); ++column)
            std::memcpy(&new_matrix.data[column * TNewRows],
				&static_cast<Matrix<TRows, TColumns> const*>(this)->data[column * TRows],
				sizeof(RkFloat) * std::min(TRows, TNewRows));

		return new_matrix;
	}
};

END_RUKEN_NAMESPACE