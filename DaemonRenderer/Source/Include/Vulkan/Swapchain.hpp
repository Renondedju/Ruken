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

#include <vector>

#include "Vulkan.hpp"

BEGIN_DAEMON_NAMESPACE

class Image;
class Device;
class Window;
class Instance;
class Semaphore;

/**
 * \brief A swapchain object provides the ability to present rendering results to a surface.
 * \note  A native window cannot be associated with more than one non-retired swapchain at a time.
 *        Further, swapchains cannot be created for native windows that have a non-Vulkan graphics API surface associated with them.
 */
class Swapchain
{
    private:

        #pragma region Members

        Instance const&                 m_instance;
        Device   const&                 m_device;
        VkSurfaceKHR                    m_surface;
        VkSwapchainKHR                  m_handle;
        DAEuint32                       m_image_count;
        VkSurfaceFormatKHR              m_surface_format;
        VkExtent2D                      m_image_extent;
        VkSurfaceTransformFlagBitsKHR   m_pre_transform;
        VkCompositeAlphaFlagBitsKHR     m_composite_alpha;
        VkPresentModeKHR                m_present_mode;
        std::vector<Image>              m_images;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Selects the minimum number of presentable images that the application needs.
         *
         *  \note The implementation will either create the swapchain with at least that many images, or it will fail to create the swapchain.
         */
        DAEvoid SelectImageCount(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the format the swapchain image(s) will be created with.
         */
        DAEvoid SelectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& in_available_formats) noexcept;

        /**
         * \brief Selects the size (in pixels) of the swapchain image(s).
         *
         * \note The behavior is platform-dependent if the image extent does not match the surface�fs 'currentExtent'.
         */
        DAEvoid SelectImageExtent(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the value describing the transform, relative to the presentation engine�fs natural orientation, applied to the image content prior to presentation.
         *
         * \note If it does not match the 'currentTransform' value, the presentation engine will transform the image content as part of the presentation operation.
         */
        DAEvoid SelectPreTransform(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the value indicating the alpha compositing mode to use when this surface is composited together with other surfaces on certain window systems.
         */
        DAEvoid SelectCompositeAlpha(VkSurfaceCapabilitiesKHR const& in_capabilities) noexcept;

        /**
         * \brief Selects the presentation mode the swapchain will use.
         *
         * \note A swapchain�fs present mode determines how incoming present requests will be processed and queued internally.
         */
        DAEvoid SelectPresentMode(std::vector<VkPresentModeKHR> const& in_available_present_modes) noexcept;

        DAEbool SetupSurface(Instance const& in_instance, Window const& in_window);

        /**
         * \return True if a a swapchain could be created, else False.
         */
        DAEbool SetupSwapchain(VkSwapchainKHR in_old_swapchain = nullptr);

        DAEbool SetupImages();

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        explicit Swapchain(Instance const&  in_instance,
                           Device   const&  in_device,
                           Window   const&  in_window);

        Swapchain(Swapchain const&  in_copy) = delete;
        Swapchain(Swapchain&&       in_move) = delete;

        ~Swapchain() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Recreates the swapchain whenever the window is resized.
         *
         * \param in_width  The new width (in pixels) of the window.
         * \param in_height The new height (in pixels) of the window.
         */
        [[nodiscard]]
        DAEbool ResizeSwapchain(DAEint32 in_width, DAEint32 in_height);

        [[nodiscard]]
        DAEbool AcquireNextImage(Semaphore const& in_semaphore, DAEuint32& out_image_index) const noexcept;

        /**
         * \return The opaque handle to the swapchain object.
         */
        [[nodiscard]]
        VkSwapchainKHR GetHandle() const noexcept;

        [[nodiscard]]
        DAEuint32 GetImageCount() const noexcept;

        [[nodiscard]]
        std::vector<Image> const& GetImages() const noexcept;

        #pragma endregion

        #pragma region Operators

        Swapchain& operator=(Swapchain const&   in_copy) = delete;
        Swapchain& operator=(Swapchain&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE