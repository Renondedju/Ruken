#pragma once

#include "JobSystem/Awaitables/AsyncAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * The shared mutual exclusion structure is a synchronization primitive that can be used to protect shared data
 * from being simultaneously accessed by multiple threads. A SharedMutex has two levels of access:
 *   • shared    (ReadAccess)  - several threads can share ownership of the same mutex.
 *   • exclusive (WriteAccess) - only one thread can own the mutex.
 *
 * @tparam TData Data type to be protected
 */
template <typename TData>
struct Mutex
{
	#pragma region Lifetime

	explicit Mutex() requires std::default_initializable<TData>;
	explicit Mutex(TData const& in_data);
	Mutex(Mutex const&)            = delete;
	Mutex(Mutex&&)                 = delete;
	Mutex& operator=(Mutex const&) = delete;
	Mutex& operator=(Mutex&&)      = delete;
	~Mutex()					   = default;

	#pragma endregion

	#pragma region Access

	struct Access
	{
		explicit Access() = default;
		explicit Access(Mutex&) noexcept;
		 Access(Access const&) = delete;
		 Access(Access&&     )	 noexcept;
		~Access()		  		 noexcept;

		Access&      operator=(Access const&) = delete;
		Access&      operator=(Access&&     ) noexcept;
		TData&		 operator* ()	    const noexcept;
		TData&		 operator->()	    const noexcept;

	protected:

		Mutex* m_mutex {};
	};

	#pragma endregion

	#pragma region Methods

	/// @brief Returns an awaitable that can be used to access the underlying data.
	auto operator co_await() const noexcept;

	#pragma endregion

	private:

		#pragma region Awaitables

		struct MutexAwaiter: AsyncAwaiter
		{
			Mutex* mutex;

			RkBool await_ready  ()						  const noexcept;
			RkBool await_suspend(std::coroutine_handle<>)       noexcept;
			Access await_resume ()						  const noexcept;
		};

		struct MutexAwaitable: AsyncAwaitable
		{
			Mutex* mutex;

			/**
			 * @brief Signals & consumes the attached awaiters until in_predicate returns false.
			 * @param in_predicate Predicate indicating if the passed awaiter should be consumed.
			 * @returns True if the method consumed any awaiters.
			 */
			template <std::predicate<AsyncAwaiter*> TPredicate>
			RkUint64 SignalConsumeIf(TPredicate&& in_predicate) const noexcept;

			/// @returns An awaiter.
			MutexAwaiter operator co_await() const;
		};

		#pragma endregion

		friend Access;

		#pragma region Members

		TData		     m_data;
		std::atomic_flag m_locked    {};
		MutexAwaitable   m_awaitable {
			{}, this
		};

		#pragma endregion

		#pragma region Methods

		/**
		* Consumes the following awaiters if any.
		* The method is thread safe and will only trigger if concurrency equals 0.
		*/
		RkVoid ConsumeNext() noexcept;

		/**
		 * Checks if an awaiter can be signaled based on the current state of the mutex.
		 * @param in_awaiter Awaiter to check for. (unused)
		 * @return True if the awaiter can be signaled.
		 */
		RkBool CanSignal(AsyncAwaiter const* in_awaiter) const noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE

#include "Core/JobSystem/Awaitables/Primitives/Mutex.inl"
