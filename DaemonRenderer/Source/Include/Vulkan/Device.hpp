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

#include "Vulkan/Vulkan.hpp"

#include <thread>

#include "Containers/Vector.hpp"
#include "Containers/UnorderedMap.hpp"

BEGIN_DAEMON_NAMESPACE

class Image;
class Queue;
class Buffer;
class Instance;
class CommandPool;
class PipelineCache;
class PhysicalDevice;

typedef Vector<Queue*> QueueFamily;
typedef UnorderedMap<Queue*, CommandPool*> QueueCommandPools;

/**
 * \brief This class wraps a VkDevice object.
 *
 * A logical device represents an instance of physical device implementation with its own state and resources independent of other logical devices.
 *
 * \note Vulkan separates the concept of physical and logical devices.
 */
class Device
{
    private:

        #pragma region Members

        PhysicalDevice* m_physical_device;
        VkDevice        m_handle;
        VmaAllocator    m_memory_allocator;
        PipelineCache*  m_pipeline_cache;

        Vector<QueueFamily> m_queues;

        UnorderedMap<std::thread::id, QueueCommandPools> m_command_pools;

        #pragma endregion

        #pragma region Constructor

        explicit Device(PhysicalDevice* in_physical_device);

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_physical_device 
         * 
         * \return 
         */
        static DAEbool CheckDeviceExtensions(PhysicalDevice* in_physical_device) noexcept;

        /**
         * \param in_physical_device 
         * 
         * \return 
         */
        static DAEuint32 RateDeviceSuitability(PhysicalDevice* in_physical_device) noexcept;

        /**
         * \return True if a new device instance could be created, else False.
         */
        DAEbool SetupDevice() noexcept;

        /**
         * \return 
         */
        DAEbool SetupMemoryAllocator() noexcept;

        /**
         * \brief 
         */
        DAEvoid SetupQueues();

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Device() = delete;

        Device(Device const&    in_copy) = delete;
        Device(Device&&         in_move) = delete;

        ~Device() noexcept;

        #pragma endregion

        #pragma region Operators

        Device& operator=(Device const& in_copy) = delete;
        Device& operator=(Device&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_instance 
         * \param out_device  
         * 
         * \return 
         */
        static DAEbool Create(Instance* in_instance, Device** out_device);

        /**
         * \brief 
         *
         * \param in_device 
         */
        static DAEvoid Destroy(Device* in_device);

        /**
         * \param out_buffer 
         * 
         * \return 
         */
        DAEbool CreateBuffer(Buffer** out_buffer);

        /**
         * \brief 
         *
         * \param in_buffer 
         */
        DAEvoid DestroyBuffer(Buffer* in_buffer) const noexcept;

        /**
         * \param out_image 
         * 
         * \return 
         */
        DAEbool CreateImage(Image** out_image);

        /**
         * \brief 
         *
         * \param in_image 
         */
        DAEvoid DestroyImage(Image* in_image) const noexcept;

        /**
         * \brief 
         */
        [[nodiscard]] DAEvoid WaitIdle() const noexcept;

        /**
         * \return The opaque handle to the device object.
         */
        [[nodiscard]] PhysicalDevice* GetPhysicalDevice() const noexcept;

        /**
         * \return The opaque handle to the device object.
         */
        [[nodiscard]] VkDevice GetHandle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] PipelineCache* GetPipelineCache() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Vector<QueueFamily> const& GetQueueFamilies() const noexcept;

        /**
         * \brief 
         *
         * \param in_queue_family_index 
         * \param in_queue_in_index     
         * 
         * \return 
         */
        [[nodiscard]] Queue* GetQueue(DAEuint32 in_queue_family_index, DAEuint32 in_queue_in_index) const noexcept;

        /**
         * \brief 
         *
         * \param in_queue_flags    
         * \param in_queue_in_index 
         * 
         * \return 
         */
        [[nodiscard]] Queue* GetQueueByFlags(VkQueueFlags in_queue_flags, DAEuint32 in_queue_in_index) const noexcept;

        /**
         * \param in_queue 
         * 
         * \return 
         */
        [[nodiscard]] CommandPool* GetCommandPool(Queue* in_queue);

        #pragma endregion 
};

END_DAEMON_NAMESPACE