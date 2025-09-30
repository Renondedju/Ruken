#pragma once

#include "Rendering/Coroutines/GPUPromise.hpp"

BEGIN_RUKEN_NAMESPACE

struct GPUTask
{
	using promise_type = GPUPromise;

	#pragma region Lifetime

	explicit GPUTask(GPUPromise& in_promise) noexcept;
	GPUTask(GPUTask const&) 		   noexcept;
	GPUTask(GPUTask&&)				   noexcept;
	GPUTask& operator=(GPUTask const&) noexcept;
	GPUTask& operator=(GPUTask&&)      noexcept;
	~GPUTask()						   noexcept;

	#pragma endregion

	#pragma region Methods

	Awaiter operator co_await() const noexcept;

	#pragma endregion

	private:

		#pragma region Members

		GPUPromise* m_handle;

		#pragma endregion

		#pragma region Methods

		/// @brief Increments the references counter by one.
		RkVoid MakeReference  () const noexcept;

		/// @brief Decrements the references counter by one and destroys the promise if needed.
		RkVoid RemoveReference() const noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE