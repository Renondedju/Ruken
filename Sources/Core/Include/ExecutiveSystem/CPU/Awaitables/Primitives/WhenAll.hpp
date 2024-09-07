#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp"

BEGIN_RUKEN_NAMESPACE

struct WhenAll: CountDownLatch
{
	template <typename TAwaitable>
	explicit WhenAll(std::vector<TAwaitable> const& in_awaitables) noexcept:
		CountDownLatch  {in_awaitables.size()},
		m_continuations {in_awaitables.size()}
	{
		for (int i = 0; i < in_awaitables.size(); i++)
		{
			m_continuations[i].Setup(in_awaitables[i], *this);
			if (!m_continuations[i].TryStartAwait())
				Signal();
		}
	}

	private:

		std::vector<CPUContinuation<RkVoid>> m_continuations;
};

END_RUKEN_NAMESPACE