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

#include <vector>

#include "Vulkan/Core/Image.hpp"
#include "Vulkan/Core/Queue.hpp"
#include "Vulkan/Core/Buffer.hpp"
#include "Vulkan/Core/PipelineCache.hpp"
#include "Vulkan/Core/PhysicalDevice.hpp"

BEGIN_DAEMON_NAMESPACE

class Instance;

/**
 * \brief A logical device represents an instance of physical device implementation
 *        with its own state and resources independent of other logical devices.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class Device
{
    private:

        #pragma region Members

        PhysicalDevice const*   m_physical_device   {nullptr};
        VkDevice                m_handle            {nullptr};
        VmaAllocator            m_allocator         {nullptr};

        std::unique_ptr<PipelineCache> m_pipeline_cache;

        std::vector<std::vector<Queue>> m_queues;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        static DAEbool CheckDeviceExtensions(PhysicalDevice const& in_physical_device) noexcept;

        /**
         * \return 
         */
        static DAEuint32 RateDeviceSuitability(PhysicalDevice const& in_physical_device) noexcept;

        /**
         * \return 
         */
        DAEbool PickPhysicalDevice(Instance const& in_instance) noexcept;

        /**
         * \return 
         */
        DAEbool CreateDevice();

        /**
         * \return 
         */
        DAEbool CreateMemoryAllocator() noexcept;

        /**
         * \return 
         */
        DAEvoid SetupQueues();

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Device() = default;

        Device(Device const&    in_copy) = delete;
        Device(Device&&         in_move) = delete;

        ~Device() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        DAEbool Initialize(Instance const& in_instance);

        /**
         * \brief 
         */
        DAEvoid WaitIdle() const noexcept;

        [[nodiscard]]
        Image CreateImage(VkImageCreateInfo       const& in_image_create_info,
                          VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept;

        [[nodiscard]]
        Buffer CreateBuffer(VkBufferCreateInfo      const& in_buffer_create_info,
                            VmaAllocationCreateInfo const& in_allocation_create_info) const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        PhysicalDevice const* GetPhysicalDevice() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkDevice const& GetHandle() const noexcept;

        [[nodiscard]]
        PipelineCache& GetPipelineCache() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        Queue const& RequestGraphicsQueue() const;

        /**
         * \return 
         */
        [[nodiscard]]
        Queue const& RequestComputeQueue() const;

        /**
         * \return 
         */
        [[nodiscard]]
        Queue const& RequestTransferQueue() const;

        /**
         * \return 
         */
        [[nodiscard]]
        Queue const& RequestPresentQueue(VkSurfaceKHR const& in_surface) const;

        #pragma endregion

        #pragma region Operators

        Device& operator=(Device const& in_copy) = delete;
        Device& operator=(Device&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE