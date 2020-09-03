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
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

BEGIN_RUKEN_NAMESPACE

class Scheduler;

/**
 * \brief RAII-class wrapping a 'VkDevice' object.
 *        A logical device represents an instance of physical device implementation
 *        with its own state and resources independent of other logical devices.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class VulkanDevice
{
    private:

        #pragma region Members

        VkDevice                 m_handle         {nullptr};
        QueueFamilyIndices       m_queue_families {};
        std::vector<VulkanQueue> m_queues         {};

        /**
         * \brief A command pool is dedicated to a queue family (first key) and to a thread (second key),
         *        because they cannot be used concurrently.
         */
        std::unordered_map<DAEuint32, std::unordered_map<std::thread::id, VulkanCommandPool>> m_command_pools {};

        std::unique_ptr<VulkanPipelineCache> m_pipeline_cache {};

        #pragma endregion

        #pragma region Methods

        DAEbool CreateDevice      (VulkanPhysicalDevice const& in_physical_device) noexcept;
        DAEvoid CreateQueues      (VulkanPhysicalDevice const& in_physical_device) noexcept;
        DAEvoid CreateCommandPools(Scheduler            const& in_scheduler)       noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanDevice(Scheduler            const& in_scheduler,
                              VulkanPhysicalDevice const& in_physical_device) noexcept;

        VulkanDevice(VulkanDevice const& in_copy) = delete;
        VulkanDevice(VulkanDevice&&      in_move) = delete;

        ~VulkanDevice() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Waits on the host for the completion of outstanding queue operations for all queues on this device.
         */
        DAEvoid WaitIdle() const noexcept;

        /**
         * \return A queue family supporting presentation to the given surface.
         */
        [[nodiscard]] std::optional<DAEuint32> FindPresentFamily(VkSurfaceKHR const& in_surface) const noexcept;

        /**
         * \return A pointer to a queue supporting presentation to the given surface.
         */
        [[nodiscard]] VulkanQueue const* RequestPresentQueue(VkSurfaceKHR const& in_surface) const noexcept;

        [[nodiscard]] DAEbool                    IsValid               () const noexcept;
        [[nodiscard]] DAEuint32                  GetGraphicsFamily     () const noexcept;
        [[nodiscard]] DAEuint32                  GetComputeFamily      () const noexcept;
        [[nodiscard]] DAEuint32                  GetTransferFamily     () const noexcept;
        [[nodiscard]] VkDevice            const& GetHandle             () const noexcept;
        [[nodiscard]] VulkanQueue         const& GetGraphicsQueue      () const noexcept;
        [[nodiscard]] VulkanQueue         const& GetComputeQueue       () const noexcept;
        [[nodiscard]] VulkanQueue         const& GetTransferQueue      () const noexcept;
        [[nodiscard]] VulkanCommandPool   const& GetGraphicsCommandPool() const noexcept;
        [[nodiscard]] VulkanCommandPool   const& GetComputeCommandPool () const noexcept;
        [[nodiscard]] VulkanCommandPool   const& GetTransferCommandPool() const noexcept;
        [[nodiscard]] VulkanPipelineCache const& GetPipelineCache      () const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanDevice& operator=(VulkanDevice const& in_copy) = delete;
        VulkanDevice& operator=(VulkanDevice&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE