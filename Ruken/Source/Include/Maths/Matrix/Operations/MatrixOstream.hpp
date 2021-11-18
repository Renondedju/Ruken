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