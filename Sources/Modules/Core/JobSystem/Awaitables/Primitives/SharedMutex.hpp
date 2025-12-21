#pragma once

#include "JobSystem/Awaitables/AsyncAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

enum class EAccessType: RkUint64
{ Read = 0, Write = 1 };

/**
 * The shared mutual exclusion structure is a synchronization primitive that can be used to protect shared data
 * from being simultaneously accessed by multiple threads. A SharedMutex has two levels of access:
 *   • shared    (ReadAccess)  - several threads can share ownership of the same mutex.
 *   • exclusive (WriteAccess) - only one thread can own the mutex.
 *
 * @tparam TData Data type to be protected
 */
template <typename TData>
struct SharedMutex
{
	#pragma region Lifetime

	/// @brief Forwards all passed arguments to TData(TArgs...)
	template <typename... TArgs> requires std::constructible_from<TData, TArgs...>
	explicit SharedMutex(TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TData, TArgs...>);
	SharedMutex			  (const SharedMutex&) = default;
	SharedMutex			  (SharedMutex&&)      = default;
	SharedMutex& operator=(const SharedMutex&) = default;
	SharedMutex& operator=(SharedMutex&&)	   = default;
	~SharedMutex()							   = default;

	#pragma endregion

	#pragma region Access

	struct ReadAccess
	{
		explicit ReadAccess() = default;
		explicit ReadAccess(SharedMutex&) noexcept;
		 ReadAccess(ReadAccess const&)	  noexcept;
		 ReadAccess(ReadAccess&&     )	  noexcept;
		~ReadAccess()		  			  noexcept;

		ReadAccess&  operator=(ReadAccess const&) noexcept;
		ReadAccess&  operator=(ReadAccess&&     ) noexcept;
		TData const& operator*()		    const noexcept;
		TData const* operator->()		    const noexcept;

	protected:

		SharedMutex* m_mutex {};
	};

	struct WriteAccess
	{
		explicit WriteAccess()			   = default;
		explicit WriteAccess(SharedMutex&) noexcept;
		 WriteAccess(WriteAccess const&)   = delete;
		 WriteAccess(WriteAccess&&     )   noexcept;
		~WriteAccess()		  			   noexcept;

		WriteAccess& operator=(WriteAccess const&) = delete;
		WriteAccess& operator=(WriteAccess&&     ) noexcept;
		TData&       operator*()                   noexcept;
		TData*		 operator->()		           noexcept;

	protected:

		SharedMutex* m_mutex {};
	};

	#pragma endregion

	#pragma region Awaitables

	struct MutexAwaitable: AsyncAwaitable
	{
		struct Awaiter: AsyncAwaiter
		{
			SharedMutex* mutex;

			RkBool await_ready  ()						  const noexcept;
			RkBool await_suspend(std::coroutine_handle<>)       noexcept;
		};

		/**
		 * @brief Signals & consumes the attached awaiters until in_predicate returns false.
		 * @param in_predicate Predicate indicating if the passed awaiter should be consumed.
		 * @returns True if the method consumed any awaiters.
		 */
		template <std::predicate<AsyncAwaiter*> TPredicate>
		RkUint64 SignalConsumeIf(TPredicate&& in_predicate) const noexcept;
	};

	struct ReadAwaitable
	{
		SharedMutex* mutex;

		struct Awaiter: MutexAwaitable::Awaiter
		{ ReadAccess await_resume() const noexcept; };

		Awaiter operator co_await() const;
	};

	struct WriteAwaitable
	{
		SharedMutex* mutex;

		struct Awaiter: MutexAwaitable::Awaiter
		{ WriteAccess await_resume() const noexcept; };

		Awaiter operator co_await() const;
	};

	#pragma endregion

	#pragma region Methods

	/// @brief Returns an awaitable to read or write to the underlying data.
	ReadAwaitable  AsyncRead () noexcept;
	WriteAwaitable AsyncWrite() noexcept;

	#pragma endregion

	private:

		friend ReadAccess;
		friend WriteAccess;

		#pragma region Members

		TData		     	m_data        {};
		std::atomic_int64_t m_concurrency {};
		MutexAwaitable      m_awaitable   {};

		#pragma endregion

		#pragma region Methods

		/**
		* Consumes the following awaiters if any.
		* The method is thread safe and will only trigger if concurrency equals 0.
		*/
		RkVoid ConsumeNext() noexcept;

		/**
		 * Checks if an awaiter can be signaled based on the current state of the mutex.
		 * @param in_awaiter Awaiter to check for.
		 * @return True if the awaiter can be signaled.
		 */
		RkBool CanSignal(AsyncAwaiter const* in_awaiter) noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE

#include "SharedMutex.inl"
