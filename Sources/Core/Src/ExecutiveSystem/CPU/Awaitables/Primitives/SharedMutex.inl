#pragma once

#include "ExecutiveSystem/CPU/Awaitables/Primitives/SharedMutex.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma region Access

template<std::default_initializable TData>
SharedMutex<TData>::Access::Access(SharedMutex& in_mutex) noexcept:
	m_mutex {&in_mutex}
{
	// Acquire is called for us before construction to ensure thread safety
}

template<std::default_initializable TData>
SharedMutex<TData>::Access::Access(Access const& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, Acquire());
}

template<std::default_initializable TData>
SharedMutex<TData>::Access::Access(Access&& in_other) noexcept:
	m_mutex {in_other.m_mutex}
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, Acquire());
}

template<std::default_initializable TData>
SharedMutex<TData>::Access::~Access() noexcept
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, Release());
}

template<std::default_initializable TData>
typename SharedMutex<TData>::Access& SharedMutex<TData>::Access::operator=(Access const& in_other) noexcept
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, Release());
	m_mutex = in_other.m_mutex;
	RUKEN_SAFE_POINTER_CALL(m_mutex, Acquire());

	return *this;
}

template<std::default_initializable TData>
typename SharedMutex<TData>::Access& SharedMutex<TData>::Access::operator=(Access&& in_other) noexcept
{
	RUKEN_SAFE_POINTER_CALL(m_mutex, Release());
	m_mutex = std::move(in_other.m_mutex);
	RUKEN_SAFE_POINTER_CALL(m_mutex, Acquire());

	return *this;
}

template<std::default_initializable TData>
TData const& SharedMutex<TData>::ReadAccess::operator*() const noexcept
{
	return Access::m_mutex->m_shared_data;
}

template<std::default_initializable TData>
TData& SharedMutex<TData>::WriteAccess::operator*() noexcept
{
	return Access::m_mutex->m_shared_data;
}

#pragma endregion

#pragma region SharedMutex

template<std::default_initializable TData>
RkVoid SharedMutex<TData>::Acquire() noexcept
{
	TracyCPlotI("concurrency", m_concurrency.fetch_add(1, std::memory_order_acq_rel) + 1);
}

template<std::default_initializable TData>
RkVoid SharedMutex<TData>::Release() noexcept
{
	auto const value = m_concurrency.fetch_sub(1, std::memory_order_acq_rel) - 1;
	TracyCPlotI("concurrency", value);
	if (value == 0)
		ConsumeNext();
}

template<std::default_initializable TData>
RkVoid SharedMutex<TData>::ConsumeNext() noexcept
{
	// This is safe because consume signals the previous awaiter only after calling this function for the next awaiter.
	auto can_consume = [&](CPUAwaiter const* in_awaiter) -> RkBool
	{
		RkBool const condition = {
			in_awaiter->tag == static_cast<RkUint64>(EAccessType::Read) ||
		   (in_awaiter->tag == static_cast<RkUint64>(EAccessType::Write) && m_concurrency.load(std::memory_order_acquire) == 0)
		};

		if (condition)
			Acquire();

		return condition;
	};

	auto count = m_awaitable.SignalConsumeIf(can_consume);

	TracyCPlotI("SignalConsumeIf()", count);
}

template<std::default_initializable TData>
template<std::predicate<CPUAwaiter*> TPredicate>
RkUint64 SharedMutex<TData>::Awaitable::SignalConsumeIf(TPredicate&& in_predicate) const noexcept
{
	RkUint64		count	  {0ULL};
	CPUAwaiterList* selection {&m_awaiter_list};
	CPUAwaiter*		previous  {nullptr};
	CPUAwaiter*   	continuation;

	while(true)
	{
		// Waiting for a lock on the selection and acquiring it as soon as possible
		while ((continuation = selection->exchange(CPUAwaiter::locked, std::memory_order_acq_rel)) == CPUAwaiter::locked)
			_mm_pause();

		// Return if nothing can or should be consumed
		if (continuation == nullptr || !in_predicate(continuation))
		{
			if (previous != nullptr) {
				previous->next  .store(CPUAwaiter::consumed, std::memory_order_release);
				previous->signal.Signal();
				count++;
			}

			// Updating the list head and releasing the lock
			m_awaiter_list.store(continuation, std::memory_order_release);

			return count;
		}

		// This is done this way in case it gets destroyed as a side effect
		// and avoids us to read potentially unallocated memory in the code above.
		if (previous != nullptr) {
			previous->next  .store(CPUAwaiter::consumed, std::memory_order_release);
			previous->signal.Signal();
			count++;
		}

		// Fetching the next awaiter
		selection = &continuation->next;
		previous  = continuation;
	}
}

template<std::default_initializable TData>
RkBool SharedMutex<TData>::Awaiter::await_ready() const noexcept
{
	return false;
}

template<std::default_initializable TData>
RkBool SharedMutex<TData>::Awaiter::await_suspend(std::coroutine_handle<>) noexcept
{
	CPUAwaiterList* selection {head};
	CPUAwaiter*     continuation;

	TracyMessageL("Start Attach");

	// Waiting for a lock on the head and acquiring it as soon as possible
	while ((continuation = selection->exchange(locked, std::memory_order_acq_rel)) == locked)
		_mm_pause();

	// Starting the "consume chain reaction" if it wasn't started already without actually attaching to the mutex
	if (continuation == nullptr && mutex->m_concurrency.load(std::memory_order_acquire) == 0)
	{
		mutex->Acquire();
		head ->store(nullptr, std::memory_order_release);

		TracyMessageL("No attach, manual start");
		return false;
	}

	// Getting to the end of the list and wait for locks in the process
	CPUAwaiter* first {continuation};
	while (continuation != nullptr)
	{
		if (continuation != locked && continuation != detached && continuation != consumed)
			selection = &continuation->next;

		continuation = selection->load(std::memory_order_acquire);
	}

	// Finally, attaching and releasing the lock
	selection->store(this , std::memory_order_release);
	head	 ->store(first, std::memory_order_release);

	TracyMessageL("Attached, auto start");
	return true;
}

template<std::default_initializable TData>
template<typename TAccess>
auto SharedMutex<TData>::MakeAwaitable(EAccessType in_access_type) noexcept
{
	/// Creating the structure acting as the lock
	struct SharedMutexAwaitable: Awaitable
	{
		auto operator co_await() const
		{
			/// Composing
			struct SharedMutexAwaiter: Awaiter
			{
				auto await_resume() const noexcept
				{ return TAccess(*this->mutex); }

			} awaiter { this->mutex->m_awaitable.operator co_await(), this->mutex };

			awaiter.tag = static_cast<RkUint64>(this->access_type);

			return awaiter;
		}
	};

	return SharedMutexAwaitable {
		Awaitable {
			.mutex		 = this,
			.access_type = in_access_type
		}
	};
}

#pragma endregion

END_RUKEN_NAMESPACE
