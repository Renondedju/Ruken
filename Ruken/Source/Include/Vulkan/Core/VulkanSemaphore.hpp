
#pragma once

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a binary 'VkSemaphore' object.
 *        Semaphores are a synchronization primitive that can be used to insert a dependency
 *        between queue operations or between a queue operation and the host.
 *        Binary semaphores have two states - signaled and unsignaled.
 *        A semaphore can be signaled after execution of a queue operation is completed,
 *        and a queue operation can wait for a semaphore to become signaled before it begins execution.
 */
class VulkanSemaphore
{
    protected:

        #pragma region Members

        VkSemaphore m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        VulkanSemaphore() noexcept;

        VulkanSemaphore(VulkanSemaphore const& in_copy) = delete;
        VulkanSemaphore(VulkanSemaphore&&      in_move) noexcept;

        virtual ~VulkanSemaphore() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VkSemaphore const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanSemaphore& operator=(VulkanSemaphore const& in_copy) = delete;
        VulkanSemaphore& operator=(VulkanSemaphore&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE