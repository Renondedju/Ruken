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

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Matrix/Operations/MatrixAccess.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix ostream left shift operator
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixOstream
{
	/**
     * \brief ostream left shift operator overload for the GenericMatrix class
     * 
     * \param inout_stream Output stream 
     * \param in_matrix Matrix to display
     * 
     * \return Output stream instance
     */
    friend std::ostream& operator<<(std::ostream& inout_stream, Matrix<TRows, TColumns> const& in_matrix)
    {
        inout_stream << std::setprecision(3) << std::fixed;
        for (RkSize row {0ULL}; row < TRows; ++row)
        {
            for (RkSize column {0ULL}; column < TColumns; ++column)
                inout_stream << ' ' << MatrixAccess::At(in_matrix, row, column);
            inout_stream << '\n';
        }

        return inout_stream;
    }
};

END_RUKEN_NAMESPACE