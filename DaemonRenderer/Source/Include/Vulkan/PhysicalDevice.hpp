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

#include "Vulkan.hpp"

#include "Containers/Vector.hpp"

BEGIN_DAEMON_NAMESPACE

class Instance;

/**
 * \brief This class wraps a VkPhysicalDevice object.
 *
 * A physical device usually represents a single complete implementation of Vulkan (excluding instance-level functionality) available to the host, of which there are a finite number.
 *
 * \note Vulkan separates the concept of physical and logical devices.
 */
class PhysicalDevice
{
    private:

        #pragma region Members

        Instance* m_instance;

        VkPhysicalDevice m_handle;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        PhysicalDevice() = delete;

        explicit PhysicalDevice(Instance* in_instance, VkPhysicalDevice in_handle) noexcept;

        PhysicalDevice(PhysicalDevice const&    in_copy) noexcept = delete;
        PhysicalDevice(PhysicalDevice&&         in_move) noexcept = default;

        ~PhysicalDevice() noexcept = default;

        #pragma endregion

        #pragma region Operators

        PhysicalDevice& operator=(PhysicalDevice const& in_copy) noexcept = delete;
        PhysicalDevice& operator=(PhysicalDevice&&      in_move) noexcept = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] Instance* GetInstance() const noexcept;

        [[nodiscard]] VkPhysicalDevice GetHandle() const noexcept;

        [[nodiscard]] VkPhysicalDeviceProperties GetProperties() const noexcept;

        [[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const noexcept;

        [[nodiscard]] VkPhysicalDeviceMemoryProperties GetMemoryProperties() const noexcept;

        [[nodiscard]] Vector<VkQueueFamilyProperties> GetQueueFamilyProperties() const noexcept;

        #pragma endregion 
};

END_DAEMON_NAMESPACE