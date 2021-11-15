
#include <array>

#include "Vulkan/Core/VulkanSwapchain.hpp"

#include "Windowing/Window.hpp"

#include "Rendering/RenderFrame.hpp"

#include "Vulkan/Core/VulkanQueue.hpp"
#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

#ifdef RUKEN_OS_WINDOWS
    #include "Utility/WindowsOS.hpp"
#endif

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanSwapchain::VulkanSwapchain(VulkanPhysicalDevice& in_physical_device,
                                 VulkanDevice&         in_device,
                                 Window&               in_window) noexcept:
    m_physical_device {in_physical_device},
    m_device          {in_device}
{
    auto const extent = in_window.GetFramebufferSize();

    m_image_extent.width  = static_cast<RkUint32>(extent.width);
    m_image_extent.height = static_cast<RkUint32>(extent.height);

    if (CreateSurface(in_device, in_window) && SetupSwapchain() && CreateSwapchain())
    {
        CreateImages();

        in_window.on_framebuffer_resized += [this](RkInt32 const in_width, RkInt32 const in_height)
        {
            RecreateSwapchain(in_width, in_height);
        };
    }
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

RkVoid VulkanSwapchain::SelectImageCount(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    m_image_count = in_capabilities.minImageCount + 1u;

    if (in_capabilities.maxImageCount > 0u && in_capabilities.maxImageCount < m_image_count)
    {
        m_image_count = in_capabilities.maxImageCount;
    }
}

RkVoid VulkanSwapchain::SelectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept
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

        if (available_format.format     == VK_FORMAT_B8G8R8A8_SRGB &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            m_surface_format = available_format;
            break;
        }
    }
}

RkVoid VulkanSwapchain::SelectImageExtent(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.currentExtent.width != UINT32_MAX)
    {
        m_image_extent = in_capabilities.currentExtent;
    }

    // m_image_extent.width  = Clamp(m_image_extent.width,  in_capabilities.minImageExtent.width,  in_capabilities.maxImageExtent.width);
    // m_image_extent.height = Clamp(m_image_extent.height, in_capabilities.minImageExtent.height, in_capabilities.maxImageExtent.height);
}

RkVoid VulkanSwapchain::SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
{
    if (in_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        m_pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    else
        m_pre_transform = in_capabilities.currentTransform;
}

RkVoid VulkanSwapchain::SelectCompositeAlpha(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept
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

RkVoid VulkanSwapchain::SelectPresentMode(std::vector<VkPresentModeKHR> const& in_available_present_modes) noexcept
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

RkBool VulkanSwapchain::CreateSurface(VulkanDevice& in_device, Window& in_window) noexcept
{
    #ifdef RUKEN_OS_WINDOWS

    VkWin32SurfaceCreateInfoKHR surface_create_info = {};

    surface_create_info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_create_info.hinstance = GetModuleHandle(nullptr);
    surface_create_info.hwnd      = in_window.GetWin32Window();

    if (VK_CHECK(vkCreateWin32SurfaceKHR(VulkanLoader::GetLoadedInstance(), &surface_create_info, nullptr, &m_surface)))
        return false;

    #else

    (RkVoid)in_device;
    (RkVoid)in_window;

    return false;

    #endif

    m_queue = in_device.RequestPresentQueue(m_surface);

    if (!m_queue)
    {
        VulkanDebug::Error("No queue supports presentation to the surface!");

        return false;
    }

    return true;
}

RkBool VulkanSwapchain::SetupSwapchain() noexcept
{
    VkSurfaceCapabilitiesKHR capabilities;

    if (VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device.GetHandle(), m_surface, &capabilities)))
        return false;

    auto count = 0u;

    if (VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device.GetHandle(), m_surface, &count, nullptr)))
        return false;

    std::vector<VkSurfaceFormatKHR> formats(count);

    if (VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device.GetHandle(), m_surface, &count, formats.data())))
        return false;

    if (VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device.GetHandle(), m_surface, &count, nullptr)))
        return false;

    std::vector<VkPresentModeKHR> present_modes(count);

    if (VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device.GetHandle(), m_surface, &count, present_modes.data())))
        return false;

    SelectImageCount    (capabilities);
    SelectSurfaceFormat (formats);
    SelectImageExtent   (capabilities);
    SelectPreTransform  (capabilities);
    SelectCompositeAlpha(capabilities);
    SelectPresentMode   (present_modes);

    return true;
}

RkBool VulkanSwapchain::CreateSwapchain(VkSwapchainKHR in_old_swapchain) noexcept
{
    VkSwapchainCreateInfoKHR swapchain_info = {};

    swapchain_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.surface          = m_surface;
    swapchain_info.minImageCount    = m_image_count;
    swapchain_info.imageFormat      = m_surface_format.format;
    swapchain_info.imageColorSpace  = m_surface_format.colorSpace;
    swapchain_info.imageExtent      = m_image_extent;
    swapchain_info.imageArrayLayers = 1u;
    swapchain_info.imageUsage       = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    std::array<RkUint32, 2> queue_families_indices = {
        m_device.GetGraphicsFamily(),
        m_device.FindPresentFamily(m_surface).value_or(UINT64_MAX)
    };

    if (queue_families_indices[0] != queue_families_indices[1])
    {
        swapchain_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchain_info.queueFamilyIndexCount = static_cast<RkUint32>(queue_families_indices.size());
        swapchain_info.pQueueFamilyIndices   = queue_families_indices.data();
    }

    swapchain_info.preTransform   = m_pre_transform;
    swapchain_info.compositeAlpha = m_composite_alpha;
    swapchain_info.presentMode    = m_present_mode;
    swapchain_info.clipped        = VK_TRUE;
    swapchain_info.oldSwapchain   = in_old_swapchain;

    if (VK_CHECK(vkCreateSwapchainKHR(VulkanLoader::GetLoadedDevice(), &swapchain_info, nullptr, &m_handle)))
        return false;

    return true;
}

RkVoid VulkanSwapchain::CreateImages() noexcept
{
    // Obtains the number of presentable images associated with the swapchain.
    VK_CHECK(vkGetSwapchainImagesKHR(VulkanLoader::GetLoadedDevice(), m_handle, &m_image_count, nullptr));

    std::vector<VkImage> images(m_image_count);

    // Obtains the array of presentable images associated with the swapchain.
    VK_CHECK(vkGetSwapchainImagesKHR(VulkanLoader::GetLoadedDevice(), m_handle, &m_image_count, images.data()));

    VkExtent3D extent = {
        m_image_extent.width,
        m_image_extent.height,
        1u
    };

    for (auto i = 0u; i < m_image_count; ++i)
    {
        m_images.emplace_back(images[i], m_surface_format.format, extent);

        VulkanDebug::SetObjectName(VK_OBJECT_TYPE_IMAGE, reinterpret_cast<RkUint64>(images[i]), "SwapchainImage_" + std::to_string(i));
    }
}

RkVoid VulkanSwapchain::RecreateSwapchain(RkInt32 const in_width, RkInt32 const in_height) noexcept
{
    m_image_extent.width  = in_width;
    m_image_extent.height = in_height;

    if (m_image_extent.width  == 0 || m_image_extent.height == 0)
        return;

    m_images.clear();

    if (SetupSwapchain() && CreateSwapchain(m_handle))
        CreateImages();
}

RkVoid VulkanSwapchain::Present(RenderFrame& in_frame) noexcept
{
    if (m_image_extent.width  == 0 || m_image_extent.height == 0)
        return;

    auto& image_available_semaphore   = in_frame.GetSemaphorePool().RequestSemaphore();
    auto& transfer_finished_semaphore = in_frame.GetSemaphorePool().RequestSemaphore();

    if (VK_CHECK(vkAcquireNextImageKHR(VulkanLoader::GetLoadedDevice(),
                                       m_handle,
                                       UINT64_MAX,
                                       image_available_semaphore.GetHandle(),
                                       nullptr,
                                       &m_image_index)))
        return;

    auto* command_buffer = in_frame.GetGraphicsCommandPool().RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    if (!command_buffer)
        return;

    auto const& src_image = in_frame.GetRenderTarget().GetImage();
    auto const& dst_image = m_images[m_image_index];

    if (!command_buffer->Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
        return;

    VkImageMemoryBarrier memory_barrier = {};

    memory_barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    memory_barrier.srcQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
    memory_barrier.dstQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
    memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    memory_barrier.subresourceRange.levelCount = 1u;
    memory_barrier.subresourceRange.layerCount = 1u;

    // ...
    memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    memory_barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    memory_barrier.image         = src_image.GetHandle();

    command_buffer->InsertMemoryBarrier(0u, 0u, VK_DEPENDENCY_BY_REGION_BIT, memory_barrier);

    // ...
    memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    memory_barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    memory_barrier.image         = dst_image.GetHandle();

    command_buffer->InsertMemoryBarrier(0u, 0u, VK_DEPENDENCY_BY_REGION_BIT, memory_barrier);

    command_buffer->BlitImage(src_image, dst_image, VK_FILTER_NEAREST);

    // ...
    memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    memory_barrier.newLayout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    memory_barrier.image         = src_image.GetHandle();

    command_buffer->InsertMemoryBarrier(0u, 0u, VK_DEPENDENCY_BY_REGION_BIT, memory_barrier);

    // ...
    memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    memory_barrier.newLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    memory_barrier.image         = dst_image.GetHandle();

    command_buffer->InsertMemoryBarrier(0u, 0u, VK_DEPENDENCY_BY_REGION_BIT, memory_barrier);

    if (!command_buffer->End())
        return;

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

    if (!m_device.GetGraphicsQueue().Submit(submit_info))
        VulkanDebug::Error("");

    if (!m_queue->Present(*this, transfer_finished_semaphore))
        VulkanDebug::Error("Failed to queue an image for presentation!");
}

RkBool VulkanSwapchain::IsValid() const noexcept
{
    return m_surface && m_queue && m_handle && !m_images.empty();
}

VkSwapchainKHR const& VulkanSwapchain::GetHandle() const noexcept
{
    return m_handle;
}

RkUint32 const& VulkanSwapchain::GetImageIndex() const noexcept
{
    return m_image_index;
}

RkUint32 VulkanSwapchain::GetImageCount() const noexcept
{
    return m_image_count;
}

#pragma endregion