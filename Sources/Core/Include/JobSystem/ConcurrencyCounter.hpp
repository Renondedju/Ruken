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


static constexpr ConcurrencyCounter s_one_optimal { .current_concurrency = 0, .optimal_concurrency = 1 };
static constexpr ConcurrencyCounter s_one_current { .current_concurrency = 0, .optimal_concurrency = 1 };

static_assert(sizeof(ConcurrencyCounter) == sizeof(RkUint64));

END_RUKEN_NAMESPACE
