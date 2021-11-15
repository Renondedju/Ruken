
#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Provides the strong types suffixes for the ostream operator<<
 * \tparam TStrongType Strong type
 */
template <typename TStrongType>
struct StrongTypeSuffix
{
    static constexpr const RkChar* suffix = "";
};

END_RUKEN_NAMESPACE