#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp"

BEGIN_RUKEN_NAMESPACE

struct WhenAll: CountDownLatch
{
	template <typename TAwaitable>
	explicit WhenAll(std::vector<TAwaitable> const& in_awaitables) noexcept:
		CountDownLatch {in_awaitables.size()},
		m_awaiters     {in_awaitables.size()}
	{
		for (int i = 0; i < in_awaitables.size(); i++)
		{
			m_awaiters[i].signal = CPUSignal<RkVoid>(*this);
			in_awaitables[i].AttachOrSignal(m_awaiters[i]);
		}
	}

	private:

		std::vector<CPUAwaiter<RkVoid>> m_awaiters;
};

END_RUKEN_NAMESPACE