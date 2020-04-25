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

#include "Vulkan/Core/VulkanSwapchain.hpp"
#include "Vulkan/Core/VulkanQueue.hpp"
#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

#include "Windowing/Window.hpp"

#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderSystem.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

VulkanSwapchain::VulkanSwapchain(VulkanPhysicalDevice& in_physical_device, VulkanDevice& in_device, Window& in_window):
    m_physical_device   {in_physical_device.GetHandle()},
    m_image_extent      {in_window.GetFramebufferSize()}
{
    if (CreateSurface(in_device, in_window) && CreateSwapchain())
        CreateImages();
}

VulkanSwapchain::~VulkanSwapchain() noexcept
{
    if (m_handle)
        vkDestroySwapchainKHR(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);

    if (m_surface)
        vkDestroySurfaceKHR(VulkanLoader::GetLoadedInstance(), m_surface, nullptr);
}

#pragma endregion

#pragma region Methods

DAEvoid VulkanSwapchain::SelectImageCount(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    m_image_count = in_capabilities.minImageCount + 1u;

    if (in_capabilities.maxImageCount > 0u && in_capabilities.maxImageCount < m_image_count)
    {
        m_image_count = in_capabilities.maxImageCount;
    }
}

DAEvoid VulkanSwapchain::SelectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept
{
    // Best case scenario, the surface has no preferred format.
    if (in_available_formats.size() == 1u && in_available_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        m_surface_format.format     = VK_FORMAT_B8G8R8A8_SRGB;
        m_surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return;
    }

    m_surface_format = in_available_formats[0];

    // Checks if the preferred combination is available.
    for (auto const& available_format : in_available_formats)
    {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB)
            m_surface_format = available_format;

        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB  && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            m_surface_format = available_format;
            break;
        }
    }
}

DAEvoid VulkanSwapchain::SelectImageExtent(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.currentExtent.width != UINT32_MAX)
    {
        m_image_extent = in_capabilities.currentExtent;
    }

    //m_image_extent.width  = Clamp(m_image_extent.width,  in_capabilities.minImageExtent.width,  in_capabilities.maxImageExtent.width);
    //m_image_extent.height = Clamp(m_image_extent.height, in_capabilities.minImageExtent.height, in_capabilities.maxImageExtent.height);
}

DAEvoid VulkanSwapchain::SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        m_pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }

    else
        m_pre_transform = in_capabilities.currentTransform;
}

DAEvoid VulkanSwapchain::SelectCompositeAlpha(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    std::vector<VkCompositeAlphaFlagBitsKHR> composite_alphas = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
    };

    for (auto const& composite_alpha : composite_alphas)
    {
        if (in_capabilities.supportedCompositeAlpha & composite_alpha)
        {
            m_composite_alpha = composite_alpha;
            break;
        }
    }
}

DAEvoid VulkanSwapchain::SelectPresentMode(std::vector<VkPresentModeKHR> const& in_available_present_modes) noexcept
{
    m_present_mode = VK_PRESENT_MODE_FIFO_KHR;

    for (auto const& available_present_mode : in_available_present_modes)
    {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            m_present_mode = available_present_mode;
            return;
        }
    }
}

DAEbool VulkanSwapchain::CreateSurface(VulkanDevice& in_device, Window& in_window) noexcept
{
    if (glfwCreateWindowSurface(VulkanLoader::GetLoadedInstance(), in_window.GetHandle(), nullptr, &m_surface) == VK_SUCCESS)
    {
        m_queue = in_device.GetPresentQueue(m_surface);

        if (m_queue)
            return true;

        VulkanDebug::GetLogger().Error("No queue supports presentation to the surface!");
    }

    else
        VulkanDebug::GetLogger().Error("Failed to create surface!");

    return false;
}

DAEbool VulkanSwapchain::CreateSwapchain(VkSwapchainKHR in_old_swapchain) noexcept
{
    VkSurfaceCapabilitiesKHR capabilities;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, m_surface, &capabilities);

    DAEuint32 count = 0u;

    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, m_surface, &count, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, m_surface, &count, formats.data());

    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, m_surface, &count, nullptr);

    std::vector<VkPresentModeKHR> present_modes(count);

    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, m_surface, &count, present_modes.data());

    SelectImageCount    (capabilities);
    SelectSurfaceFormat (formats);
    SelectImageExtent   (capabilities);
    SelectPreTransform  (capabilities);
    SelectCompositeAlpha(capabilities);
    SelectPresentMode   (present_modes);

    VkSwapchainCreateInfoKHR swapchain_info = {};

    swapchain_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.surface          = m_surface;
    swapchain_info.minImageCount    = m_image_count;
    swapchain_info.imageFormat      = m_surface_format.format;
    swapchain_info.imageColorSpace  = m_surface_format.colorSpace;
    swapchain_info.imageExtent      = m_image_extent;
    swapchain_info.imageArrayLayers = 1u;
    swapchain_info.imageUsage       = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    swapchain_info.preTransform     = capabilities.currentTransform;
    swapchain_info.compositeAlpha   = m_composite_alpha;
    swapchain_info.presentMode      = m_present_mode;
    swapchain_info.clipped          = VK_TRUE;
    swapchain_info.oldSwapchain     = in_old_swapchain;

   return vkCreateSwapchainKHR(VulkanLoader::GetLoadedDevice(), &swapchain_info, nullptr, &m_handle) == VK_SUCCESS;
}

DAEvoid VulkanSwapchain::CreateImages()
{
    // Obtains the number of presentable images associated with the swapchain.
    if (vkGetSwapchainImagesKHR(VulkanLoader::GetLoadedDevice(), m_handle, &m_image_count, nullptr) != VK_SUCCESS)
    {
        VulkanDebug::GetLogger().Error("Failed to create swapchain images!");
        return;
    }

    std::vector<VkImage> images(m_image_count);

    // Obtains the array of presentable images associated with the swapchain.
    vkGetSwapchainImagesKHR(VulkanLoader::GetLoadedDevice(), m_handle, &m_image_count, images.data());

    for (DAEuint32 i = 0; i < m_image_count; ++i)
    {
        m_images.emplace_back(images[i]);

        VulkanDebug::SetObjectName(VK_OBJECT_TYPE_IMAGE, reinterpret_cast<DAEuint64>(images[i]), "SwapchainImage_" + std::to_string(i));
    }
}

DAEvoid VulkanSwapchain::Resize(DAEint32 const in_width, DAEint32 const in_height)
{
    if (in_width == 0 || in_height == 0)
        return;

    m_image_extent.width  = in_width;
    m_image_extent.height = in_height;

    m_images.clear();

    if (CreateSwapchain(m_handle))
        CreateImages();
}

DAEvoid VulkanSwapchain::Present(RenderFrame& in_frame) const noexcept
{
    auto& device = GRenderSystem->GetDevice();
    auto& image_available_semaphore = in_frame.RequestSemaphore();
    auto& transfer_finished_semaphore = in_frame.RequestSemaphore();

    DAEuint32 image_index = 0u;

    if (vkAcquireNextImageKHR(VulkanLoader::GetLoadedDevice(), m_handle, UINT64_MAX, image_available_semaphore.GetHandle(), nullptr, &image_index) != VK_SUCCESS)
    {
        VulkanDebug::GetLogger().Error("");
        return;
    }

    auto* command_buffer = in_frame.RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VkImageMemoryBarrier memory_barrier = {};

    memory_barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    memory_barrier.srcAccessMask               = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    memory_barrier.dstAccessMask               = VK_ACCESS_TRANSFER_READ_BIT;
    memory_barrier.oldLayout                   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    memory_barrier.newLayout                   = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    memory_barrier.srcQueueFamilyIndex         = device.GetGraphicsFamily();
    memory_barrier.dstQueueFamilyIndex         = device.GetTransferFamily();
    memory_barrier.image                       = in_frame.GetRenderTarget().GetImage().GetHandle();
    memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    memory_barrier.subresourceRange.levelCount = 1u;
    memory_barrier.subresourceRange.layerCount = 1u;

    command_buffer->InsertMemoryBarrier(memory_barrier);

    // command_buffer->CopyImage();

    memory_barrier.srcAccessMask       = VK_ACCESS_TRANSFER_READ_BIT;
    memory_barrier.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    memory_barrier.oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    memory_barrier.newLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    memory_barrier.srcQueueFamilyIndex = device.GetTransferFamily();
    memory_barrier.dstQueueFamilyIndex = device.GetGraphicsFamily();

    command_buffer->InsertMemoryBarrier(memory_barrier);

    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;

    VkSubmitInfo submit_info = {};

    submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount   = 1u;
    submit_info.pWaitSemaphores      = &image_available_semaphore.GetHandle();
    submit_info.pWaitDstStageMask    = &wait_stage;
    submit_info.commandBufferCount   = 1u;
    submit_info.pCommandBuffers      = &command_buffer->GetHandle();
    submit_info.signalSemaphoreCount = 1u;
    submit_info.pSignalSemaphores    = &transfer_finished_semaphore.GetHandle();

    device.GetTransferQueue().Submit(submit_info);

    VkPresentInfoKHR present_info = {};

    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1u;
    present_info.pWaitSemaphores    = &transfer_finished_semaphore.GetHandle();
    present_info.swapchainCount     = 1u;
    present_info.pSwapchains        = &m_handle;
    present_info.pImageIndices      = &image_index;

    m_queue->Present(present_info);
}

DAEbool VulkanSwapchain::IsValid() const noexcept
{
    return m_surface && m_queue && m_handle && !m_images.empty();
}

#pragma endregion