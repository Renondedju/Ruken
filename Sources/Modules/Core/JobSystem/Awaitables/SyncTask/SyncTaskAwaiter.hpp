#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult>
struct SyncTaskPromise;

/// @brief An awaiter of synchronous tasks
template <typename TResult>
struct SyncTaskAwaiter
{
	#pragma region Lifetime

	explicit SyncTaskAwaiter  () = default;
	explicit SyncTaskAwaiter  (SyncTaskPromise<TResult>* in_promise) noexcept;
	SyncTaskAwaiter 		  (SyncTaskAwaiter const&) = default;
	SyncTaskAwaiter 		  (SyncTaskAwaiter&&     ) = default;
	SyncTaskAwaiter& operator=(SyncTaskAwaiter const&) = default;
	SyncTaskAwaiter& operator=(SyncTaskAwaiter&&	 ) = default;
	~SyncTaskAwaiter() noexcept;

	#pragma endregion

	#pragma region Coroutine Methods

	std::coroutine_handle<> await_suspend(std::coroutine_handle<>) noexcept;
	RkBool    			    await_ready ()					 const noexcept;
	auto					await_resume()					 const;

	#pragma endregion

	#pragma region Members

	SyncTaskPromise<TResult>* promise {nullptr};
	std::coroutine_handle<>   continuation   {};

	#pragma endregion
};

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/SyncTask/SyncTaskAwaiter.inl"