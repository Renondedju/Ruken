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

#pragma once

#include <Vulkan/vulkan.h>

#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

class Surface;
class PhysicalDevice;
class LogicalDevice;

class Swapchain
{
    private:

        #pragma region Members

        Surface        const* m_surface;
        PhysicalDevice const* m_physical_device;
        LogicalDevice  const* m_logical_device;

        VkSwapchainKHR                  m_handle;
        DAEuint32                       m_image_count;
        VkSurfaceFormatKHR              m_surface_format;
        VkExtent2D                      m_image_extent;
        VkSurfaceTransformFlagBitsKHR   m_pre_transform;
        VkCompositeAlphaFlagBitsKHR     m_composite_alpha;
        VkPresentModeKHR                m_present_mode;

        Vector<VkImage>     m_images;
        Vector<VkImageView> m_image_views;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief 
         */
        DAEvoid SelectImageCount(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief 
         */
        DAEvoid SelectSurfaceFormat(Vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept;

        /**
         * \brief 
         */
        DAEvoid SelectImageExtent(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief 
         */
        DAEvoid SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief 
         */
        DAEvoid SelectCompositeAlpha(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief 
         */
        DAEvoid SelectPresentMode(Vector<VkPresentModeKHR> const& in_available_present_modes) noexcept;

        /**
         * \brief 
         */
        DAEbool CreateSwapchain();

        /**
         * \brief 
         */
        DAEbool CreateImageViews();

        /**
         * \brief 
         */
        DAEvoid ResizeSwapchain(DAEint32 in_width, DAEint32 in_height);

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Swapchain() = delete;

        explicit Swapchain(Surface        const* in_surface,
                           PhysicalDevice const* in_physical_device,
                           LogicalDevice  const* in_logical_device);

        Swapchain(Swapchain const&    in_copy) = delete;
        Swapchain(Swapchain&&         in_move) = delete;

        ~Swapchain() noexcept;

        #pragma endregion

        #pragma region Operators

        Swapchain& operator=(Swapchain const& in_copy) = delete;
        Swapchain& operator=(Swapchain&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        [[nodiscard]] VkSwapchainKHR GetHandle() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE