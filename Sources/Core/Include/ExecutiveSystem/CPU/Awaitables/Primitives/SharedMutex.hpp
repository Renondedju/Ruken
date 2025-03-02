#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

enum class EAccessType: RkUint64
{ Read = 0, Write = 1 };

/**
 * The SharedMutex class is a synchronization primitive that can be used to protect shared data
 * from being simultaneously accessed by multiple threads. A SharedMutex has two levels of access:
 *   • shared    (ReadAccess)  - several threads can share ownership of the same mutex.
 *   • exclusive (WriteAccess) - only one thread can own the mutex.
 *
 * @tparam TData Data type to be protected
 */
template <std::default_initializable TData>
struct SharedMutex
{
	#pragma region Access

	struct Access
	{
		explicit Access() = default;
		explicit Access(SharedMutex&) noexcept;
		Access (Access const&)		  noexcept;
		Access (Access&&     )		  noexcept;
		~Access()		  			  noexcept;

		Access& operator=(Access const&) noexcept;
		Access& operator=(Access&&     ) noexcept;

	protected:

		SharedMutex* m_mutex {};
	};

	struct ReadAccess : Access { using Access::Access; TData const& operator*() const noexcept; };
	struct WriteAccess: Access { using Access::Access; TData&       operator*()       noexcept; };

	#pragma endregion

	#pragma region Methods

	/// @brief Creates an awaitable that returns access to the underlying data.
	auto AsyncRead () noexcept { return MakeAwaitable<ReadAccess >(EAccessType::Read ); }
	auto AsyncWrite() noexcept { return MakeAwaitable<WriteAccess>(EAccessType::Write); }

	/**
	 * Consumes the following awaiters if any.
	 * The method is thread safe and will only trigger if concurrency equals 0.
	 */
	RkVoid ConsumeNext() noexcept;

	#pragma endregion

	private:

		struct Awaitable: CPUAwaitable
		{
			SharedMutex* mutex;
			EAccessType  access_type;

			/**
			 * @brief Signals & consumes the attached awaiters until in_predicate returns false.
			 * @param in_predicate Predicate indicating if the passed awaiter should be consumed.
			 * @returns True if the method consumed any awaiters.
			 */
			template <std::predicate<CPUAwaiter*> TPredicate>
			RkUint64 SignalConsumeIf(TPredicate&& in_predicate) const noexcept;
		};

		struct Awaiter: CPUAwaiter
		{
			SharedMutex* mutex;

			RkBool await_ready  ()						  const noexcept;
			RkBool await_suspend(std::coroutine_handle<>)       noexcept;
		};

		#pragma region Members

		TData				 m_shared_data {};
		std::atomic_uint64_t m_concurrency {};
		Awaitable            m_awaitable   {};

		#pragma endregion

		#pragma region Methods

		friend Access;

		// Acquire/Release access
		RkVoid Acquire() noexcept;
		RkVoid Release() noexcept;

		/**
		 * Creates an async event that returns access to the underlying data
		 * @tparam TAccess Access class
		 * @param in_access_type Access type
		 * @return Awaitable instance
		 */
		template <typename TAccess>
		auto MakeAwaitable(EAccessType in_access_type) noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/Primitives/SharedMutex.inl"