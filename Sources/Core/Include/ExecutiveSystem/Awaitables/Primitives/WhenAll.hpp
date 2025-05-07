#pragma once

#include "ExecutiveSystem/Awaitables/Primitives/CountDownLatch.hpp"

#include <utility>

BEGIN_RUKEN_NAMESPACE

template <typename TAwaitable>
struct WhenAll: CountDownLatch
{
	using TAwaiter = decltype(std::declval<TAwaitable>().operator co_await());

	explicit WhenAll(std::vector<TAwaitable> const& in_awaitables) noexcept:
		CountDownLatch {in_awaitables.size()},
		m_awaiters     {in_awaitables.size()}
	{
		ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

		for (int i = 0; i < in_awaitables.size(); i++)
		{
			m_awaiters[i]		 = in_awaitables[i].operator co_await();
			m_awaiters[i].signal = ruken::Signal(*this);

			// Trying to suspend
			if (!m_awaiters[i].await_ready  () &&
				 m_awaiters[i].await_suspend(std::coroutine_handle()))
				continue;

			Signal();
		}
	}

	private:

		std::vector<TAwaiter> m_awaiters;
};

template <typename... TAwaitables>
auto WhenAllVariadic(TAwaitables const&... in_awaitables) ->
	DynamicTask<std::tuple<decltype(std::declval<decltype(std::declval<TAwaitables>().operator co_await())>().await_resume())...>>
{
	CountDownLatch															 latch    {sizeof...(TAwaitables)};
	std::tuple<decltype(std::declval<TAwaitables>().operator co_await())...> awaiters {};

	[&]<auto... Is>(std::index_sequence<Is...>)
	{
		([&](auto& in_awaiter, auto const& in_awaitable)
		{
			in_awaiter		  = in_awaitable.operator co_await();
			in_awaiter.signal = Signal(latch);

			// Trying to suspend
			if (!in_awaiter.await_ready  () &&
				 in_awaiter.await_suspend(std::coroutine_handle()))
				return;

			latch.Signal();
		}(std::get<Is>(awaiters), in_awaitables), ...);
	}(std::index_sequence_for<TAwaitables...>{});

	co_await latch;

	co_return std::apply([&](auto const&... in_values) {
		return std::tuple(in_values.await_resume()...);
	}, awaiters);
}

END_RUKEN_NAMESPACE