#pragma once

#include "Core/JobSystem/Awaitables/Primitives/CountDownLatch.hpp"
#include "Core/JobSystem/Awaitables/Tasks/DynamicTask.hpp"
#include "Core/JobSystem/Concepts/CAwaitable.hpp"

#include <utility>
#include <ranges>

BEGIN_RUKEN_NAMESPACE

template <typename TAwaitable> using AwaiterType = decltype(std::declval<TAwaitable>().operator co_await());
template <typename TAwaiter>   using ResumeType  = decltype(std::declval<TAwaiter  >().await_resume		());

template <typename TAwaitable>
struct WhenAll: CountDownLatch
{
	explicit WhenAll(std::vector<TAwaitable> const& in_awaitables) noexcept:
		CountDownLatch {in_awaitables.size()},
		m_awaiters     {in_awaitables.size()}
	{
		ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

		for (int i = 0; i < in_awaitables.size(); i++)
		{
			m_awaiters[i]		 = in_awaitables[i].operator co_await();
			m_awaiters[i].signal = SignalReceiver(*this);

			// Trying to suspend
			if (!m_awaiters[i].await_ready  () &&
				 m_awaiters[i].await_suspend(std::coroutine_handle()))
				continue;

			Signal();
		}
	}

	private:

		std::vector<AwaiterType<TAwaitable>> m_awaiters;
};

template <typename... TAwaitables>
auto WhenAllVariadic(TAwaitables const&... in_awaitables) ->
	DynamicTask<     // Runs on the same queue as the caller
		std::tuple< // And returns a tuple of all the result types of the passed awaitables
			ResumeType<AwaiterType<TAwaitables>>...
		>
	>
{
	CountDownLatch							latch    {sizeof...(TAwaitables)};
	std::tuple<AwaiterType<TAwaitables>...> awaiters {};

	// co_await compiler transform
	[&]<auto... Is>(std::index_sequence<Is...>)
	{
		([&](auto& in_awaiter, auto const& in_awaitable)
		{
			in_awaiter		  = in_awaitable.operator co_await();
			in_awaiter.signal = SignalReceiver(latch);

			// Trying to suspend
			if (!in_awaiter.await_ready  () &&
				 in_awaiter.await_suspend(std::coroutine_handle()))
				return;

			latch.Signal();
		}(std::get<Is>(awaiters), in_awaitables), ...);
	}(std::index_sequence_for<TAwaitables...>{});

	co_await latch;

	// Gathering results
	co_return std::apply([&] <typename... TAwaiter> (TAwaiter&... in_awaiters) {
		return std::tuple(std::forward<ResumeType<TAwaiter>>(in_awaiters.await_resume())...);
	}, awaiters);
}

END_RUKEN_NAMESPACE