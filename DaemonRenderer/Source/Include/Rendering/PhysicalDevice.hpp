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
    VkSurfaceCapabilitiesKHR capabilities = {};

    Vector<VkSurfaceFormatKHR>  formats;
    Vector<VkPresentModeKHR>    present_modes;
};

/**
 * \brief This class wraps a VkPhysicalDevice object.
 *
 * A physical device usually represents a single complete implementation of Vulkan (excluding instance-level functionality) available to the host, of which there are a finite number.
 *
 * \note Vulkan separates the concept of physical and logical devices.
 */
class PhysicalDevice
{
    private:

        #pragma region Members

        VkPhysicalDevice            m_handle;
        VkSurfaceKHR                m_surface;
        VkPhysicalDeviceProperties  m_properties;
        VkPhysicalDeviceFeatures    m_features;
        QueueFamilyIndices          m_queue_families;

        Vector<DAEchar const*> m_required_extensions;
        Vector<DAEchar const*> m_required_layers;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_physical_device The physical device that will be queried.
         *
         * \return True if the required extensions are supported, else False.
         */
        DAEbool CheckDeviceExtensions(VkPhysicalDevice in_physical_device) const noexcept;

        /**
         * \param in_physical_device The physical device that will be queried.
         *
         * \return True if the required validation layers are supported, else False.
         *
         * \note In order to maintain compatibility with implementations released prior to device-layer deprecation, applications should still enumerate and enable device layers.
         */
        DAEbool CheckDeviceLayers(VkPhysicalDevice in_physical_device) const noexcept;

        /**
         * \param in_physical_device The physical device that will be queried.
         *
         * \return The indices of the required queues available on the specified physical device.
         */
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice in_physical_device) const noexcept;

        /**
         * \param in_physical_device The physical device that will be queried.
         *
         * \return The score given to the specified physical device.
         */
        DAEuint32 RateDeviceSuitability(VkPhysicalDevice in_physical_device) const;

        /**
         * \param in_instance The Vulkan instance to retrieve the physical devices from.
         *
         * \return True if a graphics card in the system that supports the features we need could be found, else False.
         */
        DAEbool SetupPhysicalDevice(VkInstance in_instance) noexcept;

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
         * \param in_physical_device The physical device that will be queried.
         * \param in_layer_name      Name of the layer to retrieve extensions from.
         *
         * \return A list of properties of available physical device extensions.
         *
         * \note When 'in_layer_name' parameter is NULL, only extensions provided by the Vulkan implementation or by implicitly enabled layers are returned.
         *       When 'in_layer_name' is the name of a layer, the device extensions provided by that layer are returned.
         */
        static Vector<VkExtensionProperties> GetSupportedExtensions(VkPhysicalDevice in_physical_device, DAEchar const* in_layer_name = nullptr) noexcept;

        /**
         * \param in_physical_device The physical device that will be queried.
         *
         * \return A list of properties of available physical device layers.
         */
        static Vector<VkLayerProperties> GetSupportedLayers(VkPhysicalDevice in_physical_device) noexcept;

        /**
         * \param in_physical_device The physical device that will be associated with the swapchain.
         * \param in_surface         The surface that will be associated with the swapchain.
         *
         * \return The properties of the surface when used by the specified physical device.
         */
        static SurfaceDetails QuerySurfaceDetails(VkPhysicalDevice in_physical_device, VkSurfaceKHR in_surface);

        /**
         * \return The opaque handle to the physical device object.
         */
        [[nodiscard]] VkPhysicalDevice GetHandle() const noexcept;

        /**
         * \return The physical device properties.
         */
        [[nodiscard]] VkPhysicalDeviceProperties GetProperties() const noexcept;

        /**
         * \return The fine-grained features supported by the physical device.
         */
        [[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const noexcept;

        /**
         * \return The indices of the required queues available on the physical device.
         */
        [[nodiscard]] QueueFamilyIndices GetQueueFamilies() const noexcept;

        /**
         * \return The properties of the surface when used by the physical device.
         */
        [[nodiscard]] SurfaceDetails GetSurfaceDetails() const;

        /**
         * \return The list of the required device extensions.
         */
        [[nodiscard]] Vector<DAEchar const*> const& GetRequiredExtensions() const noexcept;

        /**
         * \return The list of the required device validation layers.
         *
         * \note In order to maintain compatibility with implementations released prior to device-layer deprecation, applications should still enumerate and enable device layers.
         */
        [[nodiscard]] Vector<DAEchar const*> const& GetRequiredLayers() const noexcept;

        #pragma endregion 
};

END_DAEMON_NAMESPACE