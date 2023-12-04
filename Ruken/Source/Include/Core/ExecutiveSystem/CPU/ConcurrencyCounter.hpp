#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct ConcurrencyCounter
{
	union
	{
		RkUint64 value {};

	    struct
	    {
	        RkUint32 current_concurrency;
	        RkUint32 optimal_concurrency;
	    };
	};
};

static_assert(sizeof(ConcurrencyCounter) == sizeof(RkUint64));

END_RUKEN_NAMESPACE