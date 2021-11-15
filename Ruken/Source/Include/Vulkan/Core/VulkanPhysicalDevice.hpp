
#pragma once

#include <vector>
#include <optional>

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

struct QueueFamilyIndices
{
    std::optional<RkUint32> graphics;
    std::optional<RkUint32> compute;
    std::optional<RkUint32> transfer;

    [[nodiscard]]
    constexpr RkBool IsComplete() const noexcept
    {
        return graphics.has_value() && compute.has_value() && transfer.has_value();
    }
};

/**
 * \brief RAII-class wrapping a 'VkPhysicalDevice' object.
 *        A physical device usually represents a single complete implementation of Vulkan
 *        (excluding instance-level functionality) available to the host, of which there are a finite number.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class VulkanPhysicalDevice
{
    private:

        #pragma region Members

        static std::vector<RkChar const*> m_required_extensions;

        VkPhysicalDevice                     m_handle                  {nullptr};
        VkPhysicalDeviceProperties           m_properties              {};
        VkPhysicalDeviceMemoryProperties     m_memory_properties       {};
        VkPhysicalDeviceFeatures             m_features                {};
        QueueFamilyIndices                   m_queue_families          {};
        std::vector<VkQueueFamilyProperties> m_queue_family_properties {};

        #pragma endregion

        #pragma region Methods

        static RkBool   CheckDeviceExtensions(VkPhysicalDevice in_handle) noexcept;
        static RkBool   CheckQueueFamilies   (VkPhysicalDevice in_handle) noexcept;
        static RkUint32 RateDeviceSuitability(VkPhysicalDevice in_handle) noexcept;

        RkBool PickPhysicalDevice () noexcept;
        RkVoid SetupPhysicalDevice() noexcept;
        RkVoid SetupQueueFamilies () noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanPhysicalDevice() noexcept;

        VulkanPhysicalDevice(VulkanPhysicalDevice const& in_copy) = delete;
        VulkanPhysicalDevice(VulkanPhysicalDevice&&      in_move) = delete;

        ~VulkanPhysicalDevice() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        static std::vector<RkChar const*> const& GetRequiredExtensions() noexcept;

        [[nodiscard]] RkBool                                     IsValid                 () const noexcept;
        [[nodiscard]] VkPhysicalDevice                     const& GetHandle               () const noexcept;
        [[nodiscard]] VkPhysicalDeviceProperties           const& GetProperties           () const noexcept;
        [[nodiscard]] VkPhysicalDeviceMemoryProperties     const& GetMemoryProperties     () const noexcept;
        [[nodiscard]] VkPhysicalDeviceFeatures             const& GetFeatures             () const noexcept;
        [[nodiscard]] QueueFamilyIndices                   const& GetQueueFamilies        () const noexcept;
        [[nodiscard]] std::vector<VkQueueFamilyProperties> const& GetQueueFamilyProperties() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice const& in_copy) = delete;
        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE