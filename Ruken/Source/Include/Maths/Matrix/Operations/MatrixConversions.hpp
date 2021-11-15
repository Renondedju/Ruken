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

		for (RkSize row {0ULL}; row < std::min(TRows, TNewRows); ++row)
            std::memcpy(&new_matrix.data[row * TNewColumns],
				&static_cast<Matrix<TRows, TColumns> const*>(this)->data[row * TColumns],
				sizeof(RkFloat) * std::min(TColumns, TNewColumns));

		return new_matrix;
	}
};

END_RUKEN_NAMESPACE