#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TAwaitable>
struct WhenAll: CountDownLatch
{
	using TAwaiter = decltype(std::declval<TAwaitable>().operator co_await());

	explicit WhenAll(std::vector<TAwaitable> const& in_awaitables) noexcept:
		CountDownLatch {in_awaitables.size()},
		m_awaiters     {in_awaitables.size()}
	{
		for (int i = 0; i < in_awaitables.size(); i++)
		{
			m_awaiters[i]		 = in_awaitables[i].operator co_await();
			m_awaiters[i].signal = CPUSignal<typename TAwaiter::SignalValue>(*this);
			if (!m_awaiters[i].TryAttach())
				Signal();
		}
	}

	private:

		std::vector<TAwaiter> m_awaiters;
};

END_RUKEN_NAMESPACE