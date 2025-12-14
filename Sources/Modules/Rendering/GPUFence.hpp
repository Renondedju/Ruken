#pragma once

#include "Core/JobSystem/Awaitables/Primitives/ManualResetEvent.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUFence final : ManualResetEvent
{
	using Awaiter = AsyncAwaiter;

    #pragma region Lifetime

    /**
     * Constructor.
     * @param in_device Vulkan device.
     * @param in_fence_create_info Vulkan fence create info.
     */
    explicit GPUFence(vk::raii::Device const& in_device, vk::FenceCreateInfo const& in_fence_create_info);
    GPUFence           (GPUFence const&) = delete;
    GPUFence           (GPUFence &&)     = delete;
    GPUFence& operator=(GPUFence const&) = delete;
    GPUFence& operator=(GPUFence &&)     = delete;
    ~GPUFence()                          = default;

    #pragma endregion

	#pragma region Members

    /// @brief   The actual vulkan fence.
    /// @warning /!\ Do not reset manually, call GPUFence::Reset() instead.
    vk::raii::Fence			fence;
	vk::raii::Device const& device;

	#pragma endregion

	#pragma region Methods

    /// @brief Resets the fence and the underlying awaitable.
    RkVoid Reset() const noexcept;

    /// @brief Starts a wait for the fence.
    Awaiter operator co_await() const noexcept;

	/// @brief More CPU efficient wait but the running thread will be taken away from the application for a while.
	RkVoid WaitSynchronously() const noexcept;

	#pragma endregion

    private:

		#pragma region Methods

        /**
         * Checks the status of a fence and signals the event or re-schedules itself if the fence is not yet ready.
         * @param in_fence Fence to check
         * @param in_event Associated GPUFence (ManualResetEvent) to signal.
         */
        static DynamicTask<> CheckFenceStatus(vk::raii::Fence const& in_fence, GPUFence const& in_event) noexcept;

		#pragma endregion
};

END_RUKEN_NAMESPACE