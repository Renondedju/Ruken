
#pragma once

#include <thread>
#include <vector>
#include <unordered_map>

#include "Vulkan/Core/VulkanQueue.hpp"
#include "Vulkan/Core/VulkanCommandPool.hpp"
#include "Vulkan/Core/VulkanPipelineCache.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

BEGIN_RUKEN_NAMESPACE

class Scheduler;

/**
 * \brief RAII-class wrapping a 'VkDevice' object.
 *        A logical device represents an instance of physical device implementation
 *        with its own state and resources independent of other logical devices.
 * \note  Vulkan separates the concept of physical and logical devices.
 */
class VulkanDevice
{
    private:

        #pragma region Members

        VkDevice                 m_handle         {nullptr};
        QueueFamilyIndices       m_queue_families {};
        std::vector<VulkanQueue> m_queues         {};

        /**
         * \brief A command pool is dedicated to a queue family (first key) and to a thread (second key),
         *        because they cannot be used concurrently.
         */
        std::unordered_map<RkUint32, std::unordered_map<std::thread::id, VulkanCommandPool>> m_command_pools {};

        std::unique_ptr<VulkanPipelineCache> m_pipeline_cache {};

        #pragma endregion

        #pragma region Methods

        RkBool CreateDevice      (VulkanPhysicalDevice const& in_physical_device) noexcept;
        RkVoid CreateQueues      (VulkanPhysicalDevice const& in_physical_device) noexcept;
        RkVoid CreateCommandPools(Scheduler            const& in_scheduler)       noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanDevice(Scheduler            const& in_scheduler,
                              VulkanPhysicalDevice const& in_physical_device) noexcept;

        VulkanDevice(VulkanDevice const& in_copy) = delete;
        VulkanDevice(VulkanDevice&&      in_move) = delete;

        ~VulkanDevice() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Waits on the host for the completion of outstanding queue operations for all queues on this device.
         */
        RkVoid WaitIdle() const noexcept;

        /**
         * \return A queue family supporting presentation to the given surface.
         */
        [[nodiscard]] std::optional<RkUint32> FindPresentFamily(VkSurfaceKHR const& in_surface) const noexcept;

        /**
         * \return A pointer to a queue supporting presentation to the given surface.
         */
        [[nodiscard]] VulkanQueue const* RequestPresentQueue(VkSurfaceKHR const& in_surface) const noexcept;

        [[nodiscard]] RkBool                    IsValid               () const noexcept;
        [[nodiscard]] RkUint32                  GetGraphicsFamily     () const noexcept;
        [[nodiscard]] RkUint32                  GetComputeFamily      () const noexcept;
        [[nodiscard]] RkUint32                  GetTransferFamily     () const noexcept;
        [[nodiscard]] VkDevice            const& GetHandle             () const noexcept;
        [[nodiscard]] VulkanQueue         const& GetGraphicsQueue      () const noexcept;
        [[nodiscard]] VulkanQueue         const& GetComputeQueue       () const noexcept;
        [[nodiscard]] VulkanQueue         const& GetTransferQueue      () const noexcept;
        [[nodiscard]] VulkanCommandPool   const& GetGraphicsCommandPool() const noexcept;
        [[nodiscard]] VulkanCommandPool   const& GetComputeCommandPool () const noexcept;
        [[nodiscard]] VulkanCommandPool   const& GetTransferCommandPool() const noexcept;
        [[nodiscard]] VulkanPipelineCache const& GetPipelineCache      () const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanDevice& operator=(VulkanDevice const& in_copy) = delete;
        VulkanDevice& operator=(VulkanDevice&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE