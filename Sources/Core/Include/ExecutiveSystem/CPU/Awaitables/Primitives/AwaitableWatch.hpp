#pragma once

#include "Build/Namespace.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Primitives/AutomaticResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * Automatic reset event that gets triggered when the target gets awaited.
 * @tparam TAwaitable Awaitable type to watch.
 */
template <typename TAwaitable>
struct AwaitableWatch: AutomaticResetEvent
{
	TAwaitable& target;

	auto operator co_await() const noexcept
	{
		using TAwaiter = decltype(target.operator co_await());

		struct WatchAwaiter: TAwaiter
		{
			AwaitableWatch& watch;

			WatchAwaiter(TAwaitable& target, AwaitableWatch& watch):
				TAwaiter(target.operator co_await()),
				watch(watch)
			{}

			RkBool await_suspend(std::coroutine_handle<> in_handle) const noexcept
			{
				RkBool const suspend {TAwaiter::await_suspend(in_handle)};

				if (suspend)
					watch.Signal();

				return suspend;
			}
		};

		return WatchAwaiter(target, *this);
	}
};

END_RUKEN_NAMESPACE