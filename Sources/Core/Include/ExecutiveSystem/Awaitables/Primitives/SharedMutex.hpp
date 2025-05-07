#pragma once

#include "ExecutiveSystem/Awaitables/Awaitable.hpp"

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

	protected:

		SharedMutex* m_mutex {};
	};

	struct WriteAccess
	{
		explicit WriteAccess()						= default;
		explicit WriteAccess(SharedMutex&) noexcept;
		 WriteAccess(WriteAccess const&)   noexcept = delete;
		 WriteAccess(WriteAccess&&     )   noexcept;
		~WriteAccess()		  			   noexcept;

		WriteAccess& operator=(WriteAccess const&) noexcept = delete;
		WriteAccess& operator=(WriteAccess&&     ) noexcept;
		TData&       operator*()                   noexcept;

	protected:

		SharedMutex* m_mutex {};
	};

	#pragma endregion

	#pragma region Methods

	/// @brief Creates an awaitable that returns access to the underlying data.
	auto AsyncRead () noexcept { return MakeAwaitable<ReadAccess >(EAccessType::Read ); }
	auto AsyncWrite() noexcept { return MakeAwaitable<WriteAccess>(EAccessType::Write); }

	#pragma endregion

	private:

		#pragma region Awaitables

		struct MutexAwaitable: Awaitable
		{
			SharedMutex* mutex;
			EAccessType  access_type;

			/**
			 * @brief Signals & consumes the attached awaiters until in_predicate returns false.
			 * @param in_predicate Predicate indicating if the passed awaiter should be consumed.
			 * @returns True if the method consumed any awaiters.
			 */
			template <std::predicate<Awaiter*> TPredicate>
			RkUint64 SignalConsumeIf(TPredicate&& in_predicate) const noexcept;
		};

		struct MutexAwaiter: Awaiter
		{
			SharedMutex* mutex;

			RkBool await_ready  ()						  const noexcept;
			RkBool await_suspend(std::coroutine_handle<>)       noexcept;
		};

		#pragma endregion

		friend ReadAccess;
		friend WriteAccess;

		#pragma region Members

		TData		     	m_data        {};
		std::atomic_int64_t m_concurrency {};
		MutexAwaitable           m_awaitable   {};

		#pragma endregion

		#pragma region Methods

		/**
		 * Creates an async event that returns access to the underlying data
		 * @tparam TAccess Access class
		 * @param in_access_type Access type
		 * @return Awaitable instance
		 */
		template <typename TAccess>
		auto MakeAwaitable(EAccessType in_access_type) noexcept;

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
		RkBool CanSignal(Awaiter const* in_awaiter) noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/Awaitables/Primitives/SharedMutex.inl"