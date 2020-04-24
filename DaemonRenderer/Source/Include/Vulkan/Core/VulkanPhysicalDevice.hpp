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

#include "Vulkan/Utilities/VulkanLoader.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief A physical device usually represents a single complete implementation of Vulkan
 *        (excluding instance-level functionality) available to the host, of which there are a finite number.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class VulkanPhysicalDevice
{
    private:

        #pragma region Members

        VkPhysicalDevice                    m_handle            {nullptr};
        VkPhysicalDeviceProperties          m_properties        {};
        VkPhysicalDeviceMemoryProperties    m_memory_properties {};
        VkPhysicalDeviceFeatures            m_features          {};

        std::vector<VkQueueFamilyProperties> m_queue_family_properties;

        #pragma endregion

        #pragma region Methods

        static DAEbool CheckDeviceExtensions(VkPhysicalDevice in_handle) noexcept;

        static DAEbool CheckQueueFamilies(VkPhysicalDevice in_handle) noexcept;

        static DAEuint32 RateDeviceSuitability(VkPhysicalDevice in_handle) noexcept;

        DAEvoid PickPhysicalDevice(class VulkanInstance const& in_instance) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit VulkanPhysicalDevice(class VulkanInstance const& in_instance);

        VulkanPhysicalDevice(VulkanPhysicalDevice const&    in_copy) = delete;
        VulkanPhysicalDevice(VulkanPhysicalDevice&&         in_move) = default;

        ~VulkanPhysicalDevice() = default;

        #pragma endregion

        #pragma region Methods

        static std::vector<DAEchar const*> const& GetRequiredExtensions() noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkPhysicalDevice const& GetHandle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkPhysicalDeviceProperties const& GetProperties() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkPhysicalDeviceMemoryProperties const& GetMemoryProperties() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        VkPhysicalDeviceFeatures const& GetFeatures() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]]
        std::vector<VkQueueFamilyProperties> const& GetQueueFamilyProperties() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice const& in_copy) = delete;
        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE