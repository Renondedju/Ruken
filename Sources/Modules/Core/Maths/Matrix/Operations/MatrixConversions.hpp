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
	template <RkSize TNewRows, RkSize TNewColumns> explicit(TNewRows < TRows || TNewColumns < TColumns)
    operator Matrix<TNewRows, TNewColumns>() const noexcept
	{
		// Will be identity if the matrix is a square matrix
		Matrix<TNewRows, TNewColumns>  to_matrix   {};
		Matrix<TRows, TColumns> const* from_matrix {static_cast<Matrix<TRows, TColumns> const*>(this)};

		for (RkSize row {0ULL}; row < std::min(TRows, TNewRows); ++row)
            std::memcpy( // Copy is done row by row,
            	&to_matrix   .data[row * TNewColumns],
				&from_matrix->data[row * TColumns],
				sizeof(RkFloat) * std::min(TColumns, TNewColumns));

		return to_matrix;
	}
};

END_RUKEN_NAMESPACE