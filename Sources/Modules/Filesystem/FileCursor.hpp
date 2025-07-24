#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Filesystem/EFilePosition.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief A position within a file.
struct FileCursor
{
	RkSize		  offset   {0ULL};
	EFilePosition position {EFilePosition::Beginning};
};

END_RUKEN_NAMESPACE