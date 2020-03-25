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

#include "Rendering/Renderer.hpp"
#include "Vulkan/PipelineCache.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

PipelineCache::PipelineCache(VkDevice in_device) :
    m_handle { nullptr },
    m_device { in_device }
{
    VkPipelineCacheCreateInfo pipeline_cache_info = {};
    
    pipeline_cache_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipeline_cache_info.initialDataSize = 0u;
    pipeline_cache_info.pInitialData    = nullptr;

    if (vkCreatePipelineCache(m_device, &pipeline_cache_info, nullptr, &m_handle) == VK_SUCCESS)
    {
        GRenderer->GetLogger()->Info("Pipeline cache created successfully.");
    }

    else
        GRenderer->GetLogger()->Error("Failed to create pipeline cache!");

}

PipelineCache::~PipelineCache()
{
    if (m_handle)
    {
        DAEsize size;

        vkGetPipelineCacheData(m_device, m_handle, &size, nullptr);

        void* data = new char[size * 8];

        vkGetPipelineCacheData(m_device, m_handle, &size, data);

        vkDestroyPipelineCache(m_device, m_handle, nullptr);

        GRenderer->GetLogger()->Info("Pipeline cache destroyed.");
    }
}

#pragma endregion

#pragma region Methods

VkPipelineCache PipelineCache::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion