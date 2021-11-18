#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix utilities
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixUtility
{
    #pragma region Members

    static constexpr RkSize rows      = TRows;
    static constexpr RkSize columns   = TColumns;
    static constexpr RkSize elements  = TRows * TColumns;
    static constexpr RkBool is_square = TRows == TColumns;

    #pragma endregion
};

END_RUKEN_NAMESPACE