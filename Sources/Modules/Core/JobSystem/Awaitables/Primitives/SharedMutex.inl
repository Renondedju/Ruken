#pragma once

#include "Core/Meta/Safety.hpp"

#include <atomic_queue/atomic_queue.h>

BEGIN_RUKEN_NAMESPACE

template<typename TData>
template<typename ... TArgs> requires std::constructible_from<TData, TArgs...>
SharedMutex<TData>::SharedMutex(TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TData, TArgs...>):
	m_data {std::forward<TArgs>(in_args)...}
{}

#pragma region Access

template<typename TData>
SharedMutex<TData>::ReadAccess::ReadAccess(SharedMutex& in_mutex) noexcept:
	m_mutex {&in_mutex}
{
	// Acquire is called for us before construction to ensure thread safety
}

template<typename TData>
SharedMutex<TData>::ReadAccess::ReadAccess(ReadAccess const& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));
}

template<typename TData>
SharedMutex<TData>::ReadAccess::ReadAccess(ReadAccess&& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));
}

template<typename TData>
SharedMutex<TData>::ReadAccess::~ReadAccess() noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_mutex->ConsumeNext();
}

template<typename TData>
SharedMutex<TData>::ReadAccess& SharedMutex<TData>::ReadAccess::operator=(ReadAccess const& in_other) noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_mutex->ConsumeNext();

	m_mutex = in_other.m_mutex;
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));

	return *this;
}

template<typename TData>
SharedMutex<TData>::ReadAccess& SharedMutex<TData>::ReadAccess::operator=(ReadAccess&& in_other) noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_mutex->ConsumeNext();

	m_mutex = std::move(in_other.m_mutex);
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));

	return *this;
}

template<typename TData>
TData const& SharedMutex<TData>::ReadAccess::operator*() const noexcept
{
	return m_mutex->m_data;
}

template<typename TData>
TData const* SharedMutex<TData>::ReadAccess::operator->() const noexcept
{
	return &m_mutex->m_data;
}

template<typename TData>
SharedMutex<TData>::WriteAccess::WriteAccess(SharedMutex& in_mutex) noexcept:
	m_mutex {&in_mutex}
{
	// Acquire is called for us before construction to ensure thread safety
}

template<typename TData>
SharedMutex<TData>::WriteAccess::WriteAccess(WriteAccess&& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_sub(1, std::memory_order_acq_rel));
}

template<typename TData>
SharedMutex<TData>::WriteAccess::~WriteAccess() noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_add(1, std::memory_order_acq_rel) == -1)
		m_mutex->ConsumeNext();
}

template<typename TData>
SharedMutex<TData>::WriteAccess& SharedMutex<TData>::WriteAccess::operator=(WriteAccess&& in_other) noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_add(1, std::memory_order_acq_rel) == -1)
		m_mutex->ConsumeNext();

	m_mutex = std::move(in_other.m_mutex);
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_sub(1, std::memory_order_acq_rel));

	return *this;
}

template<typename TData>
TData& SharedMutex<TData>::WriteAccess::operator*() noexcept
{
	return m_mutex->m_data;
}

template<typename TData>
TData* SharedMutex<TData>::WriteAccess::operator->() noexcept
{
	return &m_mutex->m_data;
}

#pragma endregion

#pragma region SharedMutex

template<typename TData>
RkVoid SharedMutex<TData>::ConsumeNext() noexcept
{
	// This is safe because consume signals the previous awaiter only after calling this function for the next awaiter.
	m_awaitable.SignalConsumeIf([&](auto const* in_awaiter) {
		return CanSignal(in_awaiter);
	});
}

template<typename TData>
RkBool SharedMutex<TData>::CanSignal(AsyncAwaiter const* in_awaiter) noexcept
{
	// A negative concurrency represents write accesses
	// Positive concurrency represents read accesses
	RkInt64 const concurrency {m_concurrency.load(std::memory_order_acquire)};
	RkBool  const read        {in_awaiter->tag == static_cast<RkUint64>(EAccessType::Read ) && concurrency >= 0};
	RkBool  const write       {in_awaiter->tag == static_cast<RkUint64>(EAccessType::Write) && concurrency == 0};

	if (read ) m_concurrency.fetch_add(1, std::memory_order_acq_rel);
	if (write) m_concurrency.fetch_sub(1, std::memory_order_acq_rel);

	return read || write;
}

template<typename TData>
template<std::predicate<AsyncAwaiter*> TPredicate>
RkUint64 SharedMutex<TData>::MutexAwaitable::SignalConsumeIf(TPredicate&& in_predicate) const noexcept
{
	RkUint64	 	  count     {0ULL};
	AsyncAwaiterList* selection {&m_awaiter_list};
	AsyncAwaiter*     previous  {nullptr};
	AsyncAwaiter*     continuation;

	while(true)
	{
		// Waiting for a lock on the selection and acquiring it as soon as possible
		while ((continuation = selection->exchange(Awaiter::locked, std::memory_order_acq_rel)) == RUKEN_NAMESPACE::AsyncAwaiter::locked)
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
RkBool SharedMutex<TData>::MutexAwaitable::Awaiter::await_ready() const noexcept
{
	return false;
}

template<typename TData>
RkBool SharedMutex<TData>::MutexAwaitable::Awaiter::await_suspend(std::coroutine_handle<>) noexcept
{
	AsyncAwaiterList* selection {head};
	AsyncAwaiter*     continuation;

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
	if (RUKEN_NAMESPACE::AsyncAwaiter* first {continuation}; first == nullptr)
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
SharedMutex<TData>::ReadAccess SharedMutex<TData>::ReadAwaitable::Awaiter::await_resume() const noexcept
{
	return ReadAccess(*this->mutex);
}

template<typename TData>
SharedMutex<TData>::ReadAwaitable::Awaiter SharedMutex<TData>::ReadAwaitable::operator co_await() const
{
	Awaiter awaiter { mutex->m_awaitable.operator co_await(), mutex };
	awaiter.tag = static_cast<RkUint64>(EAccessType::Read);
	return awaiter;
}

template<typename TData>
SharedMutex<TData>::WriteAccess SharedMutex<TData>::WriteAwaitable::Awaiter::await_resume() const noexcept
{
	return WriteAccess(*this->mutex);
}

template<typename TData>
SharedMutex<TData>::WriteAwaitable::Awaiter SharedMutex<TData>::WriteAwaitable::operator co_await() const
{
	Awaiter awaiter { mutex->m_awaitable.operator co_await(), mutex };
	awaiter.tag = static_cast<RkUint64>(EAccessType::Write);
	return awaiter;
}

template<typename TData>
SharedMutex<TData>::ReadAwaitable SharedMutex<TData>::AsyncRead() const noexcept
{
	return ReadAwaitable {
		.mutex = this
	};
}

template<typename TData>
SharedMutex<TData>::WriteAwaitable SharedMutex<TData>::AsyncWrite() noexcept
{
	return WriteAwaitable {
		.mutex = this
	};
}

template<typename TData>
SharedMutex<TData>::ReadAwaitable SharedMutex<TData>::AsyncAccess() const noexcept
{
	return ReadAwaitable {
		.mutex = this
	};
}

template<typename TData>
SharedMutex<TData>::WriteAwaitable SharedMutex<TData>::AsyncAccess() noexcept
{
	return WriteAwaitable {
		.mutex = this
	};
}

#pragma endregion

END_RUKEN_NAMESPACE
