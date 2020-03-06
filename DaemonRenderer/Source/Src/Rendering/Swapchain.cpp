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

#include "Windowing/WindowManager.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Swapchain::Swapchain(Surface        const*  in_surface,
                     PhysicalDevice const*  in_physical_device,
                     LogicalDevice  const*  in_logical_device) :
    m_surface           { in_surface },
    m_physical_device   { in_physical_device },
    m_logical_device    { in_logical_device },
    m_handle            { nullptr },
    m_image_count       { 0u },
    m_surface_format    {},
    m_image_extent      {},
    m_pre_transform     {},
    m_composite_alpha   {},
    m_present_mode      {}
{
    SurfaceDetails const surface_details = m_physical_device->GetSurfaceDetails();

    SelectImageCount    (surface_details.capabilities);
    SelectSurfaceFormat (surface_details.formats);
    SelectImageExtent   (surface_details.capabilities);
    SelectPreTransform  (surface_details.capabilities);
    SelectCompositeAlpha(surface_details.capabilities);
    SelectPresentMode   (surface_details.present_modes);

    if (CreateSwapchain() && CreateImageViews())
    {
        GWindowManager->GetMainWindow()->on_resized.Subscribe( [this] (DAEint32 const in_width, DAEint32 const in_height) { ResizeSwapchain(in_width, in_height); });

        GRenderer->GetLogger()->Info("Swapchain created successfully.");
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create swapchain!");
}

Swapchain::~Swapchain() noexcept
{
    for (auto const& image_view : m_image_views)
    {
        vkDestroyImageView(m_logical_device->GetHandle(), image_view, nullptr);
    }

    vkDestroySwapchainKHR(m_logical_device->GetHandle(), m_handle, nullptr);

    GRenderer->GetLogger()->Info("Swapchain destroyed.");
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

DAEvoid Swapchain::SelectSurfaceFormat(Vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept
{
    // Best case scenario, the surface has no preferred format.
    if (in_available_formats.size() == 1u && in_available_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        m_surface_format.format     = VK_FORMAT_B8G8R8A8_UNORM;
        m_surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return;
    }

    m_surface_format = in_available_formats[0];

    // Checks if the preferred combination is available.
    for (auto const& available_format : in_available_formats)
    {
        if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM)
            m_surface_format = available_format;

        if (available_format.format     == VK_FORMAT_B8G8R8A8_UNORM &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
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
        return;
    }

    //m_image_extent.width  = Clamp(m_image_extent.width,  in_capabilities.minImageExtent.width,  in_capabilities.maxImageExtent.width);
    //m_image_extent.height = Clamp(m_image_extent.height, in_capabilities.minImageExtent.height, in_capabilities.maxImageExtent.height);
}

DAEvoid Swapchain::SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        m_pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
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

DAEvoid Swapchain::SelectPresentMode(Vector<VkPresentModeKHR> const& in_available_present_modes) noexcept
{
    /** TODO Checking for v-sync TODO */
    m_present_mode = VK_PRESENT_MODE_FIFO_KHR;

    for (auto const& available_present_mode : in_available_present_modes)
    {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            m_present_mode = available_present_mode;
            return;
        }

        if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            m_present_mode = available_present_mode;
    }
}

DAEbool Swapchain::CreateSwapchain()
{
    QueueFamilyIndices const queue_families  = m_physical_device->GetQueueFamilies ();

    VkSwapchainCreateInfoKHR swapchain_info;

    swapchain_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.pNext            = nullptr;
    swapchain_info.flags            = 0u;
    swapchain_info.surface          = m_surface->GetHandle();
    swapchain_info.minImageCount    = m_image_count;
    swapchain_info.imageFormat      = m_surface_format.format;
    swapchain_info.imageColorSpace  = m_surface_format.colorSpace;
    swapchain_info.imageExtent      = m_image_extent;
    swapchain_info.imageArrayLayers = 1u;
    swapchain_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (queue_families.graphics_family != queue_families.present_family)
    {
        DAEuint32 indices[] = {
            queue_families.graphics_family.value(),
            queue_families.present_family .value()
        };

        swapchain_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchain_info.queueFamilyIndexCount = 2u;
        swapchain_info.pQueueFamilyIndices   = indices;
    }

    else
    {
        swapchain_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_info.queueFamilyIndexCount = 0u;
        swapchain_info.pQueueFamilyIndices   = nullptr;
    }

    swapchain_info.preTransform   = m_pre_transform;
    swapchain_info.compositeAlpha = m_composite_alpha;
    swapchain_info.presentMode    = m_present_mode;
    swapchain_info.clipped        = VK_TRUE;
    swapchain_info.oldSwapchain   = m_handle;

    return vkCreateSwapchainKHR(m_logical_device->GetHandle(), &swapchain_info, nullptr, &m_handle) == VK_SUCCESS;
}

DAEbool Swapchain::CreateImageViews()
{
    vkGetSwapchainImagesKHR(m_logical_device->GetHandle(), m_handle, &m_image_count, nullptr);

    m_images     .resize(m_image_count);
    m_image_views.resize(m_image_count);

    vkGetSwapchainImagesKHR(m_logical_device->GetHandle(), m_handle, &m_image_count, m_images.data());

    for (DAEuint32 i = 0u; i < m_image_count; ++i)
    {
        VkImageViewCreateInfo  image_view_info;

        image_view_info.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_info.pNext    = nullptr;
        image_view_info.flags    = 0u;
        image_view_info.image    = m_images[i];
        image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_info.format   = m_surface_format.format;

        image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        image_view_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_info.subresourceRange.baseMipLevel   = 0u;
        image_view_info.subresourceRange.levelCount     = 1u;
        image_view_info.subresourceRange.baseArrayLayer = 0u;
        image_view_info.subresourceRange.layerCount     = 1u;

        if (vkCreateImageView(m_logical_device->GetHandle(), &image_view_info, nullptr, &m_image_views[i]) != VK_SUCCESS)
            return false;
    }

    return true;
}

DAEvoid Swapchain::ResizeSwapchain(DAEint32 in_width, DAEint32 in_height)
{
    /* TODO TODO */
}

VkSwapchainKHR Swapchain::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion