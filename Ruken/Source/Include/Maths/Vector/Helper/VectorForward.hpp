
#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Build/Attributes/EmptyBases.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Base un-specialized vector class (cannot be used)
 * \tparam TDimensions Number of dimensions (or size) of the vector
 * \tparam TUnderlyingType Underlying type of the vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct RUKEN_EMPTY_BASES Vector;

END_RUKEN_NAMESPACE