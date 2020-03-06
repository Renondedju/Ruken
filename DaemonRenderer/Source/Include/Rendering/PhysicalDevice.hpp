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

#include <optional>

#include <Vulkan/vulkan.h>

#include "Types/FundamentalTypes.hpp"

#include "Containers/Vector.hpp"

BEGIN_DAEMON_NAMESPACE

class Instance;
class Surface;

struct QueueFamilyIndices
{
    std::optional<DAEuint32> graphics_family;
    std::optional<DAEuint32> present_family;
    std::optional<DAEuint32> compute_family;
    std::optional<DAEuint32> transfer_family;
};

struct SurfaceDetails
{
    VkSurfaceCapabilitiesKHR    capabilities = {};
    Vector<VkSurfaceFormatKHR>  formats;
    Vector<VkPresentModeKHR>    present_modes;
};

class PhysicalDevice
{
    private:

        #pragma region Members

        Surface const* m_surface;

        VkPhysicalDevice            m_handle;
        VkPhysicalDeviceProperties  m_properties;
        VkPhysicalDeviceFeatures    m_features;
        QueueFamilyIndices          m_queue_families;
        SurfaceDetails              m_surface_details;

        Vector<DAEchar const*> m_required_extensions;
        Vector<DAEchar const*> m_required_layers;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        DAEbool CheckDeviceExtensions(VkPhysicalDevice in_physical_device) const noexcept;

        /**
         * \return 
         */
        DAEbool CheckDeviceLayers(VkPhysicalDevice in_physical_device) const noexcept;

        /**
         * \return 
         */
        DAEuint32 RateDeviceSuitability(VkPhysicalDevice in_physical_device) const;

        /**
         * \return 
         */
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice in_physical_device) const noexcept;

        /**
         *
         */
        SurfaceDetails QuerySwapchainDetails(VkPhysicalDevice in_physical_device) const;

        /**
         * \return 
         */
        DAEbool PickPhysicalDevice(VkInstance in_instance) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        PhysicalDevice() = delete;

        explicit PhysicalDevice(Instance const* in_instance,
                                Surface  const* in_surface);

        PhysicalDevice(PhysicalDevice const&    in_copy) = delete;
        PhysicalDevice(PhysicalDevice&&         in_move) = delete;

        ~PhysicalDevice() noexcept = default;

        #pragma endregion

        #pragma region Operators

        PhysicalDevice& operator=(PhysicalDevice const& in_copy) = delete;
        PhysicalDevice& operator=(PhysicalDevice&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         *
         */
        static Vector<VkExtensionProperties> GetSupportedExtensions(VkPhysicalDevice in_physical_device, DAEchar const* in_layer_name = nullptr) noexcept;

        /**
         *
         */
        static Vector<VkLayerProperties> GetSupportedLayers(VkPhysicalDevice in_physical_device) noexcept;

        /**
         * \return 
         */
        [[nodiscard]] VkPhysicalDevice GetHandle() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] VkPhysicalDeviceProperties GetProperties() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] QueueFamilyIndices GetQueueFamilies() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] SurfaceDetails GetSurfaceDetails() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Vector<DAEchar const*> const& GetRequiredExtensions() const noexcept;

        /**
         * \return 
         */
        [[nodiscard]] Vector<DAEchar const*> const& GetRequiredLayers() const noexcept;

        #pragma endregion 
};

END_DAEMON_NAMESPACE