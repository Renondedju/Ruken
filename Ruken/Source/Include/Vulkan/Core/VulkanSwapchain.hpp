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

#pragma once

#include <vector>

#include "Vulkan/Core/VulkanImage.hpp"
#include "Vulkan/Core/VulkanQueue.hpp"

BEGIN_RUKEN_NAMESPACE

class Window;
class RenderFrame;
class VulkanDevice;
class VulkanInstance;
class VulkanPhysicalDevice;

/**
 * \brief RAII-class wrapping a 'VkSwapchain' object.
 *        A swapchain object provides the ability to present rendering results to a surface.
 *        A swapchain is an abstraction for an array of presentable images that are associated with a surface.
 *        The presentable images are represented by 'VkImage' objects created by the platform.
 *        One image (which can be an array image for multiview/stereoscopic-3D surfaces) is displayed at a time,
 *        but multiple images can be queued for presentation.
 *        An application renders to the image, and then queues the image for presentation to the surface.
 * \note  A native window cannot be associated with more than one non-retired swapchain at a time.
 *        Further, swapchains cannot be created for native windows that have a non-Vulkan graphics API surface associated with them.
 */
class VulkanSwapchain
{
    private:

        #pragma region Members

        VulkanPhysicalDevice& m_physical_device;
        VulkanDevice&         m_device;

        VulkanQueue const*            m_queue           {nullptr};
        VkSurfaceKHR                  m_surface         {nullptr};
        VkSwapchainKHR                m_handle          {nullptr};
        RkUint32                     m_image_index     {0u};
        RkUint32                     m_image_count     {0u};
        VkSurfaceFormatKHR            m_surface_format  {};
        VkExtent2D                    m_image_extent    {};
        VkSurfaceTransformFlagBitsKHR m_pre_transform   {VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR};
        VkCompositeAlphaFlagBitsKHR   m_composite_alpha {VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};
        VkPresentModeKHR              m_present_mode    {VK_PRESENT_MODE_FIFO_KHR};
        std::vector<VulkanImage>      m_images          {};

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Selects the minimum number of presentable images that the application needs.
         * \note  The implementation will either create the swapchain with at least that many images,
         *        or it will fail to create the swapchain.
         */
        RkVoid SelectImageCount(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the format the swapchain image(s) will be created with.
         */
        RkVoid SelectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept;

        /**
         * \brief Selects the size (in pixels) of the swapchain image(s).
         * \note  The behavior is platform-dependent if the image extent does not match the surfaceÅ's 'currentExtent'.
         */
        RkVoid SelectImageExtent(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the value describing the transform,
         *        relative to the presentation engineÅ's natural orientation,
         *        applied to the image content prior to presentation.
         * \note  If it does not match the 'currentTransform' value,
         *        the presentation engine will transform the image content as part of the presentation operation.
         */
        RkVoid SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the value indicating the alpha compositing mode to use
         *        when this surface is composited together with other surfaces on certain window systems.
         */
        RkVoid SelectCompositeAlpha(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the presentation mode the swapchain will use.
         * \note  A swapchainÅ's present mode determines how incoming present requests will be processed and queued internally.
         */
        RkVoid SelectPresentMode(std::vector<VkPresentModeKHR> const& in_available_present_modes) noexcept;

        RkBool CreateSurface    (VulkanDevice& in_device, Window& in_window) noexcept;
        RkBool SetupSwapchain   ()                                           noexcept;
        RkBool CreateSwapchain  (VkSwapchainKHR in_old_swapchain = nullptr)  noexcept;
        RkVoid CreateImages     ()                                           noexcept;
        RkVoid RecreateSwapchain(RkInt32 in_width, RkInt32 in_height)      noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanSwapchain(VulkanPhysicalDevice& in_physical_device,
                                 VulkanDevice&         in_device,
                                 Window&               in_window) noexcept;

        VulkanSwapchain(VulkanSwapchain const& in_copy) = delete;
        VulkanSwapchain(VulkanSwapchain&&      in_move) = delete;

        ~VulkanSwapchain() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Queues the given frame for presentation.
         */
        RkVoid Present(RenderFrame& in_frame) noexcept;

        [[nodiscard]] RkBool               IsValid      () const noexcept;
        [[nodiscard]] VkSwapchainKHR const& GetHandle    () const noexcept;
        [[nodiscard]] RkUint32      const& GetImageIndex() const noexcept;
        [[nodiscard]] RkUint32             GetImageCount() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanSwapchain& operator=(VulkanSwapchain const& in_copy) = delete;
        VulkanSwapchain& operator=(VulkanSwapchain&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE