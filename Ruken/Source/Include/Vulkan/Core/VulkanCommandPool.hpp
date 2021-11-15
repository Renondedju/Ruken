
#pragma once

#include <optional>

#include "Vulkan/Core/VulkanCommandBuffer.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkCommandPool' object.
 *        Command pools are opaque objects that command buffer memory is allocated from,
 *        and which allow the implementation to amortize the cost of resource creation across multiple command buffers.
 *        Command pools are externally synchronized, meaning that a command pool must not be used concurrently in multiple threads.
 *        That includes use via recording commands on any command buffers allocated from the pool,
 *        as well as operations that allocate, free, and reset command buffers or the pool itself.
 */
class VulkanCommandPool
{
    private:

        #pragma region Members

        VkCommandPool m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanCommandPool(RkUint32 in_queue_family, VkCommandPoolCreateFlags in_flags = 0u) noexcept;

        VulkanCommandPool(VulkanCommandPool const& in_copy) = delete;
        VulkanCommandPool(VulkanCommandPool&&      in_move) noexcept;

        ~VulkanCommandPool() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return A command buffer if one could be allocated from this pool.
         * \note   The command buffers allocated using this function are single use,
         *         meaning it can submitted multiple times but only recorded once.
         */
        [[nodiscard]]
        std::optional<VulkanCommandBuffer> AllocateCommandBuffer(VkCommandBufferLevel in_level) const noexcept;

        [[nodiscard]]
        VkCommandPool const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanCommandPool& operator=(VulkanCommandPool const& in_copy) = delete;
        VulkanCommandPool& operator=(VulkanCommandPool&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE