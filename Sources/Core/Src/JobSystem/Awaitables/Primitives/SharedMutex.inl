#pragma once

#include "JobSystem/Awaitables/Primitives/SharedMutex.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma region Access

template<std::default_initializable TData>
SharedMutex<TData>::ReadAccess::ReadAccess(SharedMutex& in_mutex) noexcept:
	m_mutex {&in_mutex}
{
	// Acquire is called for us before construction to ensure thread safety
}

template<std::default_initializable TData>
SharedMutex<TData>::ReadAccess::ReadAccess(ReadAccess const& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));
}

template<std::default_initializable TData>
SharedMutex<TData>::ReadAccess::ReadAccess(ReadAccess&& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));
}

template<std::default_initializable TData>
SharedMutex<TData>::ReadAccess::~ReadAccess() noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_mutex->ConsumeNext();
}

template<std::default_initializable TData>
typename SharedMutex<TData>::ReadAccess& SharedMutex<TData>::ReadAccess::operator=(ReadAccess const& in_other) noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_mutex->ConsumeNext();

	m_mutex = in_other.m_mutex;
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));

	return *this;
}

template<std::default_initializable TData>
typename SharedMutex<TData>::ReadAccess& SharedMutex<TData>::ReadAccess::operator=(ReadAccess&& in_other) noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_sub(1, std::memory_order_acq_rel) == 1)
		m_mutex->ConsumeNext();

	m_mutex = std::move(in_other.m_mutex);
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_add(1, std::memory_order_acq_rel));

	return *this;
}

template<std::default_initializable TData>
TData const& SharedMutex<TData>::ReadAccess::operator*() const noexcept
{
	return m_mutex->m_data;
}

template<std::default_initializable TData>
SharedMutex<TData>::WriteAccess::WriteAccess(SharedMutex& in_mutex) noexcept:
	m_mutex {&in_mutex}
{
	// Acquire is called for us before construction to ensure thread safety
}

template<std::default_initializable TData>
SharedMutex<TData>::WriteAccess::WriteAccess(WriteAccess&& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_sub(1, std::memory_order_acq_rel));
}

template<std::default_initializable TData>
SharedMutex<TData>::WriteAccess::~WriteAccess() noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_add(1, std::memory_order_acq_rel) == -1)
		m_mutex->ConsumeNext();
}

template<std::default_initializable TData>
typename SharedMutex<TData>::WriteAccess& SharedMutex<TData>::WriteAccess::operator=(WriteAccess&& in_other) noexcept
{
	if (m_mutex && m_mutex->m_concurrency.fetch_add(1, std::memory_order_acq_rel) == -1)
		m_mutex->ConsumeNext();

	m_mutex = std::move(in_other.m_mutex);
	RUKEN_SAFE_POINTER_CALL(m_mutex, m_concurrency.fetch_sub(1, std::memory_order_acq_rel));

	return *this;
}

template<std::default_initializable TData>
TData& SharedMutex<TData>::WriteAccess::operator*() noexcept
{
	return m_mutex->m_data;
}

#pragma endregion

#pragma region SharedMutex

template<std::default_initializable TData>
RkVoid SharedMutex<TData>::ConsumeNext() noexcept
{
	// This is safe because consume signals the previous awaiter only after calling this function for the next awaiter.
	m_awaitable.SignalConsumeIf([&](auto const* in_awaiter) {
		return CanSignal(in_awaiter);
	});
}

template<std::default_initializable TData>
RkBool SharedMutex<TData>::CanSignal(Awaiter const* in_awaiter) noexcept
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

template<std::default_initializable TData>
template<std::predicate<Awaiter*> TPredicate>
RkUint64 SharedMutex<TData>::MutexAwaitable::SignalConsumeIf(TPredicate&& in_predicate) const noexcept
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

template<std::default_initializable TData>
RkBool SharedMutex<TData>::MutexAwaiter::await_ready() const noexcept
{
	return false;
}

template<std::default_initializable TData>
RkBool SharedMutex<TData>::MutexAwaiter::await_suspend(std::coroutine_handle<>) noexcept
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

template<std::default_initializable TData>
template<typename TAccess>
auto SharedMutex<TData>::MakeAwaitable(EAccessType in_access_type) noexcept
{
	/// Creating the structure acting as the lock
	struct AccessAwaitable: MutexAwaitable
	{
		auto operator co_await() const
		{
			/// Composing
			struct AccessAwaiter: MutexAwaiter
			{
				auto await_resume() const noexcept
				{ return TAccess(*this->mutex); }

			} awaiter { this->mutex->m_awaitable.operator co_await(), this->mutex };

			awaiter.tag = static_cast<RkUint64>(this->access_type);

			return awaiter;
		}
	};

	return AccessAwaitable {
		MutexAwaitable {
			.mutex		 = this,
			.access_type = in_access_type
		}
	};
}

#pragma endregion

END_RUKEN_NAMESPACE
