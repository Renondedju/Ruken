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

#pragma warning (push, 0)

#define VOLK_IMPLEMENTATION

#include "Vulkan/Utilities/VulkanLoader.hpp"

#pragma warning (pop)

#include "Vulkan/Utilities/VulkanDebug.hpp"

USING_DAEMON_NAMESPACE

#pragma region Methods

DAEbool VulkanLoader::Initialize() noexcept
{
    if (VK_ASSERT(volkInitialize()))
        return false;

    if (volkGetInstanceVersion() < VK_API_VERSION_1_2)
    {
        VulkanDebug::Fatal("Vulkan 1.2 is not supported!");
        return false;
    }

    return true;
}

DAEvoid VulkanLoader::LoadInstance(VkInstance in_instance) noexcept
{
    volkLoadInstanceOnly(in_instance);
}

DAEvoid VulkanLoader::LoadDevice(VkDevice in_device) noexcept
{
    volkLoadDevice(in_device);
}

VkInstance VulkanLoader::GetLoadedInstance() noexcept
{
    return volkGetLoadedInstance();
}

VkDevice VulkanLoader::GetLoadedDevice() noexcept
{
    return volkGetLoadedDevice();
}

#pragma endregion