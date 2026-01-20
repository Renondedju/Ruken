#pragma once

#include "Core/JobSystem/Awaitables/AsyncAwaiter.hpp"
#include "Core/JobSystem/Awaitables/SignalReceiver.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

template <typename TAwaitable> using AwaiterType = decltype(std::declval<TAwaitable>().operator co_await());
template <typename TAwaiter>   using ResumeType  = decltype(std::declval<TAwaiter  >().await_resume		());

// TODO: implement a "sync" coroutine type that executes in place to ease implementation of these primitives.
template <typename TAwaitable>
struct SleepWait
{
	explicit SleepWait(TAwaitable const& in_awaitable);

	ResumeType<AwaiterType<TAwaitable>>& Result() noexcept;

	RkVoid Signal() noexcept;

	private:

			ResumeType<AwaiterType<TAwaitable>> m_result		 {};
			std::atomic_flag				    m_wait_condition {};

};

template<typename TAwaitable>
SleepWait<TAwaitable>::SleepWait(TAwaitable const& in_awaitable)
{
	AwaiterType<TAwaitable> awaiter {in_awaitable.operator co_await()};
	awaiter.signal = SignalReceiver(*this);

	if (!awaiter.await_ready() && awaiter.await_suspend({}))
		m_wait_condition.wait(false);

	m_result = awaiter.await_resume();
}

template<typename TAwaitable>
ResumeType<AwaiterType<TAwaitable>>& SleepWait<TAwaitable>::Result() noexcept
{
	return m_result;
}

template<typename TAwaitable>
RkVoid SleepWait<TAwaitable>::Signal() noexcept
{
	m_wait_condition.test_and_set();
	m_wait_condition.notify_all  ();
}

END_RUKEN_NAMESPACE
