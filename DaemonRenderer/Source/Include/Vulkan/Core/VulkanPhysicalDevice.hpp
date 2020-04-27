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
#include <optional>

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_DAEMON_NAMESPACE

class VulkanInstance;

struct QueueFamilyIndices
{
    #pragma region Members

    std::optional<DAEuint32> graphics;
    std::optional<DAEuint32> compute;
    std::optional<DAEuint32> transfer;

    #pragma endregion

    #pragma region Methods

    [[nodiscard]]
    constexpr DAEbool IsComplete() const noexcept
    {
        return graphics.has_value() && compute.has_value() && transfer.has_value();
    }

    #pragma endregion
};

/**
 * \brief A physical device usually represents a single complete implementation of Vulkan
 *        (excluding instance-level functionality) available to the host, of which there are a finite number.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class VulkanPhysicalDevice
{
    private:

        #pragma region Members

        static std::vector<DAEchar const*> m_required_extensions;

        VkPhysicalDevice                    m_handle            {nullptr};
        VkPhysicalDeviceProperties          m_properties        {};
        VkPhysicalDeviceMemoryProperties    m_memory_properties {};
        VkPhysicalDeviceFeatures            m_features          {};
        QueueFamilyIndices                  m_queue_families    {};

        std::vector<VkQueueFamilyProperties> m_queue_family_properties;

        #pragma endregion

        #pragma region Methods

        static DAEbool      CheckDeviceExtensions   (VkPhysicalDevice in_handle) noexcept;
        static DAEbool      CheckQueueFamilies      (VkPhysicalDevice in_handle) noexcept;
        static DAEuint32    RateDeviceSuitability   (VkPhysicalDevice in_handle) noexcept;

        DAEvoid PickPhysicalDevice  (VulkanInstance const& in_instance) noexcept;
        DAEvoid SetupPhysicalDevice () noexcept;
        DAEvoid SetupQueueFamilies  () noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit VulkanPhysicalDevice(VulkanInstance const& in_instance) noexcept;

        VulkanPhysicalDevice(VulkanPhysicalDevice const&    in_copy) = delete;
        VulkanPhysicalDevice(VulkanPhysicalDevice&&         in_move) = delete;

        ~VulkanPhysicalDevice() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        static std::vector<DAEchar const*> const& GetRequiredExtensions() noexcept;

        [[nodiscard]]
        VkPhysicalDevice const& GetHandle() const noexcept;
        [[nodiscard]]
        VkPhysicalDeviceProperties const& GetProperties() const noexcept;
        [[nodiscard]]
        VkPhysicalDeviceMemoryProperties const& GetMemoryProperties() const noexcept;
        [[nodiscard]]
        VkPhysicalDeviceFeatures const& GetFeatures() const noexcept;
        [[nodiscard]]
        QueueFamilyIndices const& GetQueueFamilies() const noexcept;
        [[nodiscard]]
        std::vector<VkQueueFamilyProperties> const& GetQueueFamilyProperties() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice const& in_copy) = delete;
        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE