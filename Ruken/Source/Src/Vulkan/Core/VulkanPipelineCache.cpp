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

#include <fstream>

#include "Vulkan/Core/VulkanPipelineCache.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanPipelineCache::VulkanPipelineCache(std::string_view const in_path)
{
    std::ifstream file(in_path.data());

    if (!file.is_open())
        return;

    std::vector<RkChar> cache(file.seekg(0, std::ios::end).tellg());

    file.seekg(0, std::ios::beg).read(cache.data(), cache.size());

    VkPipelineCacheCreateInfo pipeline_cache_info = {};
    
    pipeline_cache_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipeline_cache_info.initialDataSize = cache.size();
    pipeline_cache_info.pInitialData    = cache.data();

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

RkVoid VulkanPipelineCache::SaveData(std::string_view const in_path) const
{
    std::ofstream file(in_path.data());

    if (!file.is_open())
        return;

    RkSize size = 0;

    if (VK_CHECK(vkGetPipelineCacheData(VulkanLoader::GetLoadedDevice(), m_handle, &size, nullptr)))
        return;

    std::vector<RkChar> cache(size);

    if (VK_CHECK(vkGetPipelineCacheData(VulkanLoader::GetLoadedDevice(), m_handle, &size, cache.data())))
        return;

    file.write(cache.data(), cache.size());
}

VkPipeline VulkanPipelineCache::CreateGraphicsPipeline(VkGraphicsPipelineCreateInfo const& in_pipeline_create_info) const noexcept
{
    VkPipeline handle = nullptr;

    if (VK_CHECK(vkCreateGraphicsPipelines(VulkanLoader::GetLoadedDevice(), m_handle, 1u, &in_pipeline_create_info, nullptr, &handle)))
        return nullptr;

    return handle;
}

VkPipeline VulkanPipelineCache::CreateComputePipeline(VkComputePipelineCreateInfo const& in_pipeline_create_info) const noexcept
{
    VkPipeline handle = nullptr;

    if (VK_CHECK(vkCreateComputePipelines(VulkanLoader::GetLoadedDevice(), m_handle, 1u, &in_pipeline_create_info, nullptr, &handle)))
        return nullptr;

    return handle;
}

std::vector<VkPipeline> VulkanPipelineCache::CreateGraphicsPipelines(std::vector<VkGraphicsPipelineCreateInfo> const& in_pipeline_create_infos) const noexcept
{
    std::vector<VkPipeline> handles(in_pipeline_create_infos.size());

    if (VK_CHECK(vkCreateGraphicsPipelines(VulkanLoader::GetLoadedDevice(),
                                           m_handle,
                                           static_cast<RkUint32>(handles.size()),
                                           in_pipeline_create_infos.data(),
                                           nullptr,
                                           handles.data())))
        return std::vector<VkPipeline>();

    return handles;
}

std::vector<VkPipeline> VulkanPipelineCache::CreateComputePipelines(std::vector<VkComputePipelineCreateInfo> const& in_pipeline_create_infos) const noexcept
{
    std::vector<VkPipeline> handles(in_pipeline_create_infos.size());

    if (VK_CHECK(vkCreateComputePipelines(VulkanLoader::GetLoadedDevice(),
                                          m_handle,
                                          static_cast<RkUint32>(handles.size()),
                                          in_pipeline_create_infos.data(),
                                          nullptr,
                                          handles.data())))
        return std::vector<VkPipeline>();

    return handles;
}

VkPipelineCache const& VulkanPipelineCache::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion