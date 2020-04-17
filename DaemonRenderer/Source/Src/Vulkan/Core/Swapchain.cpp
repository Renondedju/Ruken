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

#include "Vulkan/Core/Queue.hpp"
#include "Vulkan/Core/Device.hpp"
#include "Vulkan/Core/Swapchain.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"

#include "Windowing/Window.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Swapchain::Swapchain(Device const& in_device,
                     Window const& in_window):
    m_physical_device   {in_device.GetPhysicalDevice ()},
    m_image_extent      {in_window.GetFramebufferSize()}
{
    if (!CreateSurface(in_device, in_window))
        return;

    if (!CreateSwapchain())
        return;

    SetupImages();
}

Swapchain::~Swapchain() noexcept
{
    if (m_handle)
        vkDestroySwapchainKHR(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);

    if (m_surface)
        vkDestroySurfaceKHR(VulkanLoader::GetLoadedInstance(), m_surface, nullptr);
}

#pragma endregion

#pragma region Methods

DAEvoid Swapchain::SelectImageCount(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    m_image_count = in_capabilities.minImageCount + 1u;

    if (in_capabilities.maxImageCount > 0u && in_capabilities.maxImageCount < m_image_count)
    {
        m_image_count = in_capabilities.maxImageCount;
    }
}

DAEvoid Swapchain::SelectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept
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

DAEvoid Swapchain::SelectImageExtent(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.currentExtent.width != UINT32_MAX)
    {
        m_image_extent = in_capabilities.currentExtent;
    }

    //m_image_extent.width  = Clamp(m_image_extent.width,  in_capabilities.minImageExtent.width,  in_capabilities.maxImageExtent.width);
    //m_image_extent.height = Clamp(m_image_extent.height, in_capabilities.minImageExtent.height, in_capabilities.maxImageExtent.height);
}

DAEvoid Swapchain::SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        m_pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }

    else
        m_pre_transform = in_capabilities.currentTransform;
}

DAEvoid Swapchain::SelectCompositeAlpha(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
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

DAEvoid Swapchain::SelectPresentMode(std::vector<VkPresentModeKHR> const& in_available_present_modes) noexcept
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

DAEbool Swapchain::CreateSurface(Device const& in_device, Window const& in_window) noexcept
{
    if (VK_CHECK(glfwCreateWindowSurface(VulkanLoader::GetLoadedInstance(), in_window.GetHandle(), nullptr, &m_surface)))
    {
        m_present_queue = in_device.RequestPresentQueue(m_surface).value();

        return m_present_queue != nullptr;
    }

    return false;
}

DAEbool Swapchain::CreateSwapchain(VkSwapchainKHR in_old_swapchain) noexcept
{
    VkSurfaceCapabilitiesKHR capabilities;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device->GetHandle(), m_surface, &capabilities);

    DAEuint32 count = 0u;

    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device->GetHandle(), m_surface, &count, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device->GetHandle(), m_surface, &count, formats.data());

    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device->GetHandle(), m_surface, &count, nullptr);

    std::vector<VkPresentModeKHR> present_modes(count);

    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device->GetHandle(), m_surface, &count, present_modes.data());

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
    swapchain_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_info.preTransform     = capabilities.currentTransform;
    swapchain_info.compositeAlpha   = m_composite_alpha;
    swapchain_info.presentMode      = m_present_mode;
    swapchain_info.clipped          = VK_TRUE;
    swapchain_info.oldSwapchain     = in_old_swapchain;

    return VK_CHECK(vkCreateSwapchainKHR(VulkanLoader::GetLoadedDevice(), &swapchain_info, nullptr, &m_handle));
}

DAEvoid Swapchain::SetupImages()
{
    // Obtains the number of presentable images associated with the swapchain.
    VK_CHECK(vkGetSwapchainImagesKHR(VulkanLoader::GetLoadedDevice(), m_handle, &m_image_count, nullptr));

    m_images.resize(m_image_count);

    // Obtains the array of presentable images associated with the swapchain.
    VK_CHECK(vkGetSwapchainImagesKHR(VulkanLoader::GetLoadedDevice(), m_handle, &m_image_count, m_images.data()));

    for (DAEuint32 i = 0; i < m_image_count; ++i)
    {
        VulkanDebug::SetObjectName(VulkanLoader::GetLoadedDevice(),
                             VK_OBJECT_TYPE_IMAGE,
                             reinterpret_cast<DAEuint64>(m_images[i]),
                             "SwapchainImage_" + std::to_string(i));
    }
}

DAEvoid Swapchain::Resize(DAEint32 const in_width, DAEint32 const in_height)
{
    if (in_width == 0 || in_height == 0)
        return;

    m_image_extent.width  = in_width;
    m_image_extent.height = in_height;

    if (CreateSwapchain(m_handle))
        SetupImages();
}

VkSwapchainKHR Swapchain::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 Swapchain::GetImageCount() const noexcept
{
    return m_image_count;
}

std::vector<VkImage> const& Swapchain::GetImages() const noexcept
{
    return m_images;
}

#pragma endregion