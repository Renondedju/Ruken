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

#include "Vulkan/Image.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/Semaphore.hpp"
#include "Vulkan/Swapchain.hpp"
#include "Vulkan/PhysicalDevice.hpp"

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Swapchain::Swapchain(Instance const& in_instance,
                     Device   const& in_device,
                     Window   const& in_window):
    m_instance          {in_instance},
    m_device            {in_device},
    m_surface           {},
    m_handle            {},
    m_image_count       {0u},
    m_surface_format    {},
    m_image_extent      {in_window.GetFramebufferSize()},
    m_pre_transform     {},
    m_composite_alpha   {},
    m_present_mode      {}
{
    if (SetupSurface(in_instance, in_window) && SetupSwapchain() && SetupImages())
    {
        GRenderer->GetLogger()->Info("Swapchain created successfully.");
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create swapchain!");
}

Swapchain::~Swapchain() noexcept
{
    if (m_handle)
    {
        vkDestroySwapchainKHR(m_device.GetHandle(), m_handle, nullptr);

        GRenderer->GetLogger()->Info("Swapchain destroyed.");
    }

    if (m_surface)
    {
        vkDestroySurfaceKHR(m_instance.GetHandle(), m_surface, nullptr);

        GRenderer->GetLogger()->Info("Surface destroyed.");
    }
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

DAEbool Swapchain::SetupSurface(Instance const& in_instance, Window const& in_window)
{
    if (glfwCreateWindowSurface(m_instance.GetHandle(), in_window.GetHandle(), nullptr, &m_surface) == VK_SUCCESS)
    {
        GRenderer->GetLogger()->Info("Surface created successfully.");
        VkBool32 presentation_support;

        auto const& queue = m_device.GetPresentQueue(m_surface);

        return true;
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create surface!");

    return false;
}

DAEbool Swapchain::SetupSwapchain(VkSwapchainKHR in_old_swapchain)
{
    VkSurfaceCapabilitiesKHR capabilities;

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.GetPhysicalDevice().GetHandle(), m_surface, &capabilities) != VK_SUCCESS)
        return false;

    DAEuint32 count = 0u;

    vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.GetPhysicalDevice().GetHandle(), m_surface, &count, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(m_device.GetPhysicalDevice().GetHandle(), m_surface, &count, formats.data());

    vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.GetPhysicalDevice().GetHandle(), m_surface, &count, nullptr);

    std::vector<VkPresentModeKHR> present_modes(count);

    vkGetPhysicalDeviceSurfacePresentModesKHR(m_device.GetPhysicalDevice().GetHandle(), m_surface, &count, present_modes.data());

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

    return vkCreateSwapchainKHR(m_device.GetHandle(), &swapchain_info, nullptr, &m_handle) == VK_SUCCESS;
}

DAEbool Swapchain::SetupImages()
{
    // Obtains the array of presentable images associated with the swapchain.
    if (vkGetSwapchainImagesKHR(m_device.GetHandle(), m_handle, &m_image_count, nullptr) != VK_SUCCESS)
        return false;

    std::vector<VkImage> images(m_image_count);

    if (vkGetSwapchainImagesKHR(m_device.GetHandle(), m_handle, &m_image_count, images.data()) != VK_SUCCESS)
        return false;

    m_images.clear();

    for (auto const& image : images)
        m_images.emplace_back(m_device, image);

    return true;
}

DAEbool Swapchain::ResizeSwapchain(DAEint32 const in_width, DAEint32 const in_height)
{
    if (in_width == 0 || in_height == 0)
        return false;

    m_image_extent.width  = in_width;
    m_image_extent.height = in_height;

    return SetupSwapchain(m_handle) && SetupImages();
}

DAEbool Swapchain::AcquireNextImage(Semaphore const& in_semaphore, DAEuint32& out_image_index) const noexcept
{
    auto const result = vkAcquireNextImageKHR(m_device.GetHandle(),
                                              m_handle,
                                              ~0ull,
                                              in_semaphore.GetHandle(),
                                              nullptr,
                                              &out_image_index);

    return result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR;
}

VkSwapchainKHR Swapchain::GetHandle() const noexcept
{
    return m_handle;
}

DAEuint32 Swapchain::GetImageCount() const noexcept
{
    return m_image_count;
}

std::vector<Image> const& Swapchain::GetImages() const noexcept
{
    return m_images;
}

#pragma endregion