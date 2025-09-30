#pragma once

#include "Rendering/RenderDevice.hpp"
#include "Rendering/GPUFence.hpp"

BEGIN_RUKEN_NAMESPACE

struct GPUTask;

/// @brief Small utility struct for GPU programming
struct GPU
{
	/// @brief The current command buffer this thread is working with.
	/// This value is mainly used to communicate between coroutine promise and user content.
	static inline thread_local vk::raii::CommandBuffer* command_buffer {nullptr};
};

struct GPUPromise: ManualResetEvent
{
	#pragma region Lifetime

	template <typename... TArgs>
	explicit GPUPromise(RenderDevice& in_device, TArgs&&... in_args) noexcept;
	GPUPromise()				  = delete;
	GPUPromise(GPUPromise const&) = delete;
	GPUPromise(GPUPromise&&)      = delete;
	GPUPromise& operator=(GPUPromise const&) = delete;
	GPUPromise& operator=(GPUPromise&&)      = delete;
	~GPUPromise() = default;

	#pragma endregion

	#pragma region Members

	/// @brief Render device used for execution.
	RenderDevice&			 device;

	/// @brief Promise lifetime is reference counted.
	std::atomic_size_t		 references {1UZ};

	/// @brief Buffer containing the list of commands to execute.
	vk::raii::CommandBuffer& command_buffer;
	vk::raii::Fence			 end_execution_fence;

	#pragma endregion

	#pragma region Methods

	struct InitialSuspend;
	struct FinalSuspend;

	InitialSuspend initial_suspend  () noexcept;
	FinalSuspend   final_suspend    () noexcept;
	GPUTask		   get_return_object() noexcept;

	void unhandled_exception() noexcept;
	void await_transform    (GPUTask const& in_task) noexcept;
	void return_void        () noexcept;

	#pragma endregion
};

#pragma region Suspensions

struct GPUPromise::InitialSuspend
{
	GPUPromise* promise;

	bool await_ready () noexcept;
	void await_suspend(std::coroutine_handle<>) noexcept;
	void await_resume() noexcept;
};

struct GPUPromise::FinalSuspend
{
	GPUPromise* promise;

	bool await_ready () noexcept;
	bool await_suspend(std::coroutine_handle<>) noexcept;
	void await_resume() noexcept;

	/**
	 * A CPU Task that waits for a fence to be signaled.
	 * @param in_promise Promise to check the fence of.
	 * @param in_task Task handle to keep alive the promise.
	 * @return Async CPU task.
	 */
	static DynamicTask<> CheckFenceStatus(GPUPromise* in_promise, GPUTask in_task) noexcept;
};

#pragma endregion

END_RUKEN_NAMESPACE

#include "Rendering/Coroutines/GPUPromise.inl"
