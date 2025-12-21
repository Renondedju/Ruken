#pragma once

#include "JobSystem/Awaitables/AsyncAwaiter.hpp"

BEGIN_RUKEN_NAMESPACE

template <IsQueueHandle TQueueHandle, typename TResult>
struct AsyncTaskPromise;

/// @brief An awaiter of asynchronous tasks
template <IsQueueHandle TQueueHandle, typename TResult>
struct AsyncTaskAwaiter: AsyncAwaiter
{
	#pragma region Lifetime

	using AsyncAwaiter::operator=;

	explicit AsyncTaskAwaiter  () = default;
	explicit AsyncTaskAwaiter  (AsyncAwaiter&& in_async_awaiter, AsyncTaskPromise<TQueueHandle, TResult>* in_promise) noexcept;
	AsyncTaskAwaiter 		   (AsyncTaskAwaiter const&) = default;
	AsyncTaskAwaiter 		   (AsyncTaskAwaiter&&     ) = default;
	AsyncTaskAwaiter& operator=(AsyncTaskAwaiter const&) = default;
	AsyncTaskAwaiter& operator=(AsyncTaskAwaiter&&	   ) = default;
	~AsyncTaskAwaiter()					                 = default;

	#pragma endregion

	#pragma region Coroutine Methods

	/// @brief Returns the result of the wait
	auto await_resume() const;

	#pragma endregion

	#pragma region Members

	AsyncTaskPromise<TQueueHandle, TResult>* promise {nullptr};

	#pragma endregion
};

END_RUKEN_NAMESPACE

#include "JobSystem/Awaitables/AsyncTask/AsyncTaskAwaiter.inl"