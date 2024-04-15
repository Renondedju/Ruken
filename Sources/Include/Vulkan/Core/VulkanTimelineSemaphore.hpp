
#pragma once

#include "Vulkan/Core/VulkanSemaphore.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a timeline 'VkSemaphore' object.
 *        Semaphores are a synchronization primitive that can be used to insert a dependency
 *        between queue operations or between a queue operation and the host.
 *        Timeline semaphores have a monotonically increasing 64-bit unsigned integer payload
 *        and are signaled with respect to a particular reference value.
 *        A timeline semaphore can additionally be signaled from the host with the "Signal" command
 *        and waited on from the host with the "Wait" command.
 */
class VulkanTimelineSemaphore final : public VulkanSemaphore
{
    public:

        #pragma region Constructors

        VulkanTimelineSemaphore() noexcept;

        VulkanTimelineSemaphore(VulkanTimelineSemaphore const& in_copy) = delete;
        VulkanTimelineSemaphore(VulkanTimelineSemaphore&&      in_move) = default;

        ~VulkanTimelineSemaphore() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return True if this semaphore could be set to the given value, else False.
         */
        [[nodiscard]]
        RkBool Signal(RkUint64 in_value) const noexcept;

        /**
         * \brief If the condition is satisfied when this function is called, then it returns immediately.
         *        If the condition is not satisfied at the time this function is called,
         *        then it will block and wait for the condition to become satisfied.
         */
        [[nodiscard]]
        RkVoid Wait(RkUint64 in_value) const noexcept;

        /**
         * \return The current counter value of this semaphore or 'UINT64_MAX' if the operation failed.
         */
        [[nodiscard]]
        RkUint64 GetValue() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanTimelineSemaphore& operator=(VulkanTimelineSemaphore const& in_copy) = delete;
        VulkanTimelineSemaphore& operator=(VulkanTimelineSemaphore&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE