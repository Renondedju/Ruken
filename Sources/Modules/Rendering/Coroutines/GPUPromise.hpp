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

struct GPUPromise
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

	/// @brief Buffer containing
	vk::raii::CommandBuffer& command_buffer;

	/// @brief Promise lifetime is reference counted.
	std::atomic_size_t references {1UZ};

	/// @brief The vulkan queue this promise is recording commands for.
	//vk::QueueFlagBits vulkan_queue {vk::QueueFlagBits::eGraphics};

	/// @brief Semaphore used to wait for another task between queues.
	// std::optional<vk::raii::Semaphore> wait_task_semaphore;

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
	void await_suspend(std::coroutine_handle<>) noexcept;
	void await_resume() noexcept;
};

#pragma endregion

END_RUKEN_NAMESPACE

#include "Rendering/Coroutines/GPUPromise.inl"
