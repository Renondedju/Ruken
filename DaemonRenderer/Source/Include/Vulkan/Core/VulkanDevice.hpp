/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <thread>
#include <vector>
#include <unordered_map>

#include "Vulkan/Core/VulkanQueue.hpp"
#include "Vulkan/Core/VulkanCommandPool.hpp"
#include "Vulkan/Core/VulkanPipelineCache.hpp"

BEGIN_DAEMON_NAMESPACE

class VulkanPhysicalDevice;

/**
 * \brief A logical device represents an instance of physical device implementation
 *        with its own state and resources independent of other logical devices.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class VulkanDevice
{
    private:

        #pragma region Members

        VkDevice    m_handle            {nullptr};
        DAEuint32   m_graphics_family   {UINT32_MAX};
        DAEuint32   m_compute_family    {UINT32_MAX};
        DAEuint32   m_transfer_family   {UINT32_MAX};

        std::vector<std::vector<VulkanQueue>> m_queues;

        std::vector<std::unordered_map<std::thread::id, VulkanCommandPool>> m_command_pools;

        std::unique_ptr<VulkanPipelineCache> m_pipeline_cache;

        #pragma endregion

        #pragma region Methods

        DAEvoid CreateDevice     (VulkanPhysicalDevice const& in_physical_device);
        DAEvoid SetupQueues      (VulkanPhysicalDevice const& in_physical_device);
        DAEvoid SetupCommandPools(VulkanPhysicalDevice const& in_physical_device);

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit VulkanDevice(VulkanPhysicalDevice const& in_physical_device);

        VulkanDevice(VulkanDevice const&    in_copy) = delete;
        VulkanDevice(VulkanDevice&&         in_move) = delete;

        ~VulkanDevice() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        std::optional<VulkanCommandBuffer> AllocateCommandBuffer(DAEuint32 in_queue_family_index, VkCommandBufferLevel in_level);

        DAEvoid WaitIdle() const noexcept;

        [[nodiscard]]
        VkDevice const& GetHandle() const noexcept;

        [[nodiscard]]
        DAEuint32 GetGraphicsFamily() const noexcept;

        [[nodiscard]]
        DAEuint32 GetComputeFamily() const noexcept;

        [[nodiscard]]
        DAEuint32 GetTransferFamily() const noexcept;

        [[nodiscard]]
        DAEuint32 GetPresentFamily(VkSurfaceKHR const& in_surface) const noexcept;

        [[nodiscard]]
        VulkanQueue* GetGraphicsQueue() noexcept;

        [[nodiscard]]
        VulkanQueue* GetComputeQueue() noexcept;

        [[nodiscard]]
        VulkanQueue* GetTransferQueue() noexcept;

        [[nodiscard]]
        VulkanQueue* GetPresentQueue(VkSurfaceKHR const& in_surface) noexcept;

        [[nodiscard]]
        VulkanPipelineCache& GetPipelineCache() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanDevice& operator=(VulkanDevice const& in_copy) = delete;
        VulkanDevice& operator=(VulkanDevice&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE