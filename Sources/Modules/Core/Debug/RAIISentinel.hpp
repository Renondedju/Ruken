#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief A utility class that logs RAII operations.
struct RAIISentinel
{
	// Members
	static inline RkUint64 s_instance_index_pool {0ULL};
				  RkUint64 m_instance_index		 {s_instance_index_pool++};

	RAIISentinel();
	RAIISentinel(const RAIISentinel&);
	RAIISentinel(RAIISentinel&&);
	RAIISentinel& operator=(const RAIISentinel&);
	RAIISentinel& operator=(RAIISentinel&&);
	~RAIISentinel();
};

END_RUKEN_NAMESPACE