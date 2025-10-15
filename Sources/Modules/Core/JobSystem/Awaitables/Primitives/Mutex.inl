#pragma once

#include "Core/JobSystem/Awaitables/Primitives/Mutex.hpp"
#include "Core/Meta/Safety.hpp"

#include <atomic_queue/defs.h>

USING_RUKEN_NAMESPACE

#pragma region Access Methods

template<typename TData>
Mutex<TData>::Access::Access(Mutex& in_mutex) noexcept:
	m_mutex {&in_mutex}
{
	// Acquire is called for us before construction to ensure thread safety
}

template<typename TData>
Mutex<TData>::Access::Access(Access&& in_other) noexcept
{
	if (m_mutex)
	{
		m_mutex->m_locked.clear(std::memory_order_release);
		m_mutex->ConsumeNext();
	}

	m_mutex = in_other.m_mutex;

	RUKEN_SAFE_POINTER_CALL(m_mutex, m_locked.test_and_set(std::memory_order_acq_rel));
}

template<typename TData>
Mutex<TData>::Access::~Access() noexcept
{
	if (m_mutex)
	{
		m_mutex->m_locked.clear(std::memory_order_release);
		m_mutex->ConsumeNext();
	}
}

template<typename TData>
typename Mutex<TData>::Access& Mutex<TData>::Access::operator=(Access&& in_other) noexcept
{
	if (m_mutex)
	{
		m_mutex->m_locked.clear(std::memory_order_release);
		m_mutex->ConsumeNext();
	}

	m_mutex = in_other.m_mutex;

	RUKEN_SAFE_POINTER_CALL(m_mutex, m_locked.test_and_set(std::memory_order_acq_rel));

	return *this;
}

template<typename TData>
TData& Mutex<TData>::Access::operator*() const noexcept
{
	return m_mutex->m_data;
}

template<typename TData>
TData& Mutex<TData>::Access::operator->() const noexcept
{
	return m_mutex->m_data;
}

#pragma endregion

template<typename TData>
Mutex<TData>::Mutex() requires std::default_initializable<TData>:
	m_data {}
{}

template<typename TData>
Mutex<TData>::Mutex(TData const& in_data):
	m_data {in_data}
{}

template<typename TData>
auto Mutex<TData>::operator co_await() const noexcept
{
	return m_awaitable.operator co_await();
}

#pragma region Awaiter

template<typename TData>
RkBool Mutex<TData>::MutexAwaiter::await_ready() const noexcept
{
	return false;
}

template<typename TData>
RkBool Mutex<TData>::MutexAwaiter::await_suspend(std::coroutine_handle<>) noexcept
{
	AwaiterList* selection {head};
	Awaiter*     continuation;

	// Waiting for a lock on the head and acquiring it as soon as possible
	while ((continuation = selection->exchange(locked, std::memory_order_acq_rel)) == locked)
		atomic_queue::spin_loop_pause();

	// Fast Path: Starting the "consume chain reaction" if it wasn't started already without actually attaching
	if (continuation == nullptr && mutex->CanSignal(this))
	{
		head->store(nullptr, std::memory_order_release);
		return false;
	}

	next = nullptr;

	// Finally, attaching and releasing the lock
	if (Awaiter* first {continuation}; first == nullptr)
		head->store(this, std::memory_order_release);

	else
	{
		// Getting to the end of the list and wait for locks in the process
		while (continuation != nullptr)
		{
			selection    = &continuation->next;
			continuation = selection->load(std::memory_order_acquire);
		}

		selection->store(this,  std::memory_order_release);
		head     ->store(first, std::memory_order_release);
	}

	return true;
}

template<typename TData>
typename Mutex<TData>::Access Mutex<TData>::MutexAwaiter::await_resume() const noexcept
{
	return Access(*mutex);
}

template<typename TData>
template<std::predicate<Awaiter*> TPredicate>
RkUint64 Mutex<TData>::MutexAwaitable::SignalConsumeIf(TPredicate&& in_predicate) const noexcept
{
	RkUint64	 count	   {0ULL};
	AwaiterList* selection {&m_awaiter_list};
	Awaiter*	 previous  {nullptr};
	Awaiter*     continuation;

	while(true)
	{
		// Waiting for a lock on the selection and acquiring it as soon as possible
		while ((continuation = selection->exchange(Awaiter::locked, std::memory_order_acq_rel)) == Awaiter::locked)
			atomic_queue::spin_loop_pause();

		// Return if nothing can or should be consumed
		if (continuation == nullptr || !in_predicate(continuation))
		{
			if (previous != nullptr) {
				previous->next  .store(Awaiter::consumed, std::memory_order_release);
				previous->signal.Invoke();
				count++;
			}

			// Updating the list head and releasing the lock
			m_awaiter_list.store(continuation, std::memory_order_release);

			return count;
		}

		// This is done this way in case it gets destroyed as a side effect
		// and avoids us to read potentially unallocated memory in the code above.
		if (previous != nullptr) {
			previous->next  .store(Awaiter::consumed, std::memory_order_release);
			previous->signal.Invoke();
			count++;
		}

		// Fetching the next awaiter
		selection = &continuation->next;
		previous  = continuation;
	}
}

template<typename TData>
typename Mutex<TData>::MutexAwaiter Mutex<TData>::MutexAwaitable::operator co_await() const
{
	return MutexAwaiter { Awaiter { Awaitable::operator co_await() }, mutex };
}

template<typename TData>
RkVoid Mutex<TData>::ConsumeNext() noexcept
{
	// This is safe because consume signals the previous awaiter only after calling this function for the next awaiter.
	m_awaitable.SignalConsumeIf([&](auto const* in_awaiter) {
		return CanSignal(in_awaiter);
	});
}

template<typename TData>
RkBool Mutex<TData>::CanSignal(Awaiter const*) const noexcept
{
	return !m_locked.test(std::memory_order_acquire);
}

#pragma endregion
