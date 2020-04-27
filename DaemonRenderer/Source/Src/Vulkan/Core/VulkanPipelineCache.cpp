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

#include "Vulkan/Core/VulkanPipelineCache.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

VulkanPipelineCache::VulkanPipelineCache() noexcept
{
    VkPipelineCacheCreateInfo pipeline_cache_info = {};
    
    pipeline_cache_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipeline_cache_info.initialDataSize = 0u;
    pipeline_cache_info.pInitialData    = nullptr;

    VK_CHECK(vkCreatePipelineCache(VulkanLoader::GetLoadedDevice(), &pipeline_cache_info, nullptr, &m_handle));
}

VulkanPipelineCache::~VulkanPipelineCache() noexcept
{
    if (!m_handle)
        return;

    vkDestroyPipelineCache(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

VkPipelineCache const& VulkanPipelineCache::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion