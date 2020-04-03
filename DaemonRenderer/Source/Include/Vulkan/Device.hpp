/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Vulkan/Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Image;
class Queue;
class Buffer;
class Instance;
class PipelineCache;
class PhysicalDevice;

/**
 * \brief A logical device represents an instance of physical device implementation
 *        with its own state and resources independent of other logical devices.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class Device
{
    private:

        #pragma region Members

        PhysicalDevice const&           m_physical_device;
        VkDevice                        m_handle;
        VmaAllocator                    m_allocator;
        std::unique_ptr<PipelineCache>  m_pipeline_cache;
        std::vector<std::vector<Queue>> m_queues;

        #pragma endregion

        #pragma region Constructor


        #pragma endregion

        #pragma region Methods

        static DAEbool CheckDeviceExtensions(PhysicalDevice const& in_physical_device) noexcept;

        static DAEuint32 RateDeviceSuitability(PhysicalDevice const& in_physical_device) noexcept;

        DAEbool SetupDevice();

        DAEbool SetupMemoryAllocator() noexcept;

        DAEvoid SetupQueues();

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit Device(PhysicalDevice const& in_physical_devices);

        Device(Device const&    in_copy) = delete;
        Device(Device&&         in_move) = delete;

        ~Device() noexcept;

        #pragma endregion

        #pragma region Methods

        static DAEbool Create(Instance const& in_instance, std::unique_ptr<Device>& out_device);

        DAEvoid WaitIdle() const noexcept;

        [[nodiscard]]
        PhysicalDevice const& GetPhysicalDevice() const noexcept;

        [[nodiscard]]
        VkDevice const& GetHandle() const noexcept;

        [[nodiscard]]
        VmaAllocator const& GetAllocator() const noexcept;

        [[nodiscard]]
        PipelineCache const& GetPipelineCache() const noexcept;

        [[nodiscard]]
        Queue const& GetQueueByFlags(VkQueueFlags in_flags) const;

        [[nodiscard]]
        Queue const& GetPresentQueue(VkSurfaceKHR const& in_surface) const;

        #pragma endregion

        #pragma region Operators

        Device& operator=(Device const& in_copy) = delete;
        Device& operator=(Device&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE