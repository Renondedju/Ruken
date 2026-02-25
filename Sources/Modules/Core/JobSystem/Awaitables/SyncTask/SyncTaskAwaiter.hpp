#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult>
struct SyncTaskPromise;

/// @brief An awaiter of synchronous tasks
template <typename TResult>
struct SyncTaskAwaiter: AsyncAwaiter
{
	#pragma region Lifetime

	explicit SyncTaskAwaiter  () = default;
	explicit SyncTaskAwaiter  (AsyncAwaiter const& in_async_awaiter, SyncTaskPromise<TResult>* in_promise) noexcept;
	SyncTaskAwaiter 		  (SyncTaskAwaiter const&) = delete;
	SyncTaskAwaiter 		  (SyncTaskAwaiter&&     ) = default;
	SyncTaskAwaiter& operator=(SyncTaskAwaiter const&) = delete;
	SyncTaskAwaiter& operator=(SyncTaskAwaiter&&	 ) = default;
	~SyncTaskAwaiter()								   = default;

	#pragma endregion

	#pragma region Coroutine Methods

	decltype(auto) await_resume();

	#pragma endregion

	#pragma region Members

	SyncTaskPromise<TResult>* promise {nullptr};

	#pragma endregion
};

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.inl"