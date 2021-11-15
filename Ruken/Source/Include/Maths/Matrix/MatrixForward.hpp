
#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Build/Attributes/EmptyBases.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Matrix forward declaration
 * \tparam TRows Height or number of rows of the matrix
 * \tparam TColumns Width or number of columns of the matrix
 */
template<RkSize TRows, RkSize TColumns>
struct RUKEN_EMPTY_BASES Matrix;

END_RUKEN_NAMESPACE