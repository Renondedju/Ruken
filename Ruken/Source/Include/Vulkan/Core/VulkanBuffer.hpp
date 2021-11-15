
#pragma once

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkBuffer' object.
 *        Buffers represent linear arrays of data which are used for various purposes
 *        by binding them to a graphics or compute pipeline via descriptor sets or via certain commands,
 *        or by directly specifying them as parameters to certain commands.
 * \note  A buffer must be allocated from the VulkanDeviceAllocator and the memory will be freed at buffer's destruction.
 */
class VulkanBuffer
{
    private:

        #pragma region Members

        VkBuffer          m_handle          {nullptr};
        VmaAllocator      m_allocator       {nullptr};
        VmaAllocation     m_allocation      {nullptr};
        VmaAllocationInfo m_allocation_info {};
        RkBool           m_is_persistent   {false};
        RkBool           m_is_mapped       {false};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanBuffer(VkBuffer          in_handle,
                              VmaAllocator      in_allocator,
                              VmaAllocation     in_allocation,
                              VmaAllocationInfo in_allocation_info,
                              RkBool           in_persistent) noexcept;

        VulkanBuffer(VulkanBuffer const& in_copy) = delete;
        VulkanBuffer(VulkanBuffer&&      in_move) noexcept;

        ~VulkanBuffer() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return A pointer to the first byte of this memory if the memory was correctly mapped, else nullptr.
         * \note   This function automatically flushes or invalidates caches if necessary.
         */
        [[nodiscard]]
        RkVoid* Map() noexcept;

        /**
         * \return True if the data was correctly updated, else False.
         * \note   This function automatically maps and unmaps memory if necessary.
         */
        [[nodiscard]]
        RkBool Update(RkVoid const* in_data, RkSize in_size) noexcept;

        /**
         * \return True if the memory was correctly unmapped, else False.
         * \note   This function automatically flushes or invalidates caches if necessary.
         */
        [[nodiscard]]
        RkBool Unmap() noexcept;

        [[nodiscard]] VkBuffer       const& GetHandle() const noexcept;
        [[nodiscard]] VkDeviceMemory const& GetMemory() const noexcept;
        [[nodiscard]] VkDeviceSize   const& GetOffset() const noexcept;
        [[nodiscard]] VkDeviceSize   const& GetSize  () const noexcept;
        [[nodiscard]] RkBool               IsMapped () const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanBuffer& operator=(VulkanBuffer const& in_copy) = delete;
        VulkanBuffer& operator=(VulkanBuffer&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE