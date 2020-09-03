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

#include "Vector/Vector.hpp"

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

class VulkanImage;
class VulkanBuffer;

/**
 * \brief RAII-class wrapping a 'VkCommandBuffer' object.
 *        Command buffers are objects used to record commands which can be subsequently submitted to a device queue for execution.
 *        There are two levels of command buffers -
 *        primary command buffers, which can execute secondary command buffers, and which are submitted to queues,
 *        and secondary command buffers which can be executed by primary command buffers, and which are not directly submitted to queues.
 * \note  A command buffer must be allocated from a VulkanCommandPool and the memory will be freed at destruction.
 */
class VulkanCommandBuffer
{
    private:

        #pragma region Members

        VkCommandBuffer m_handle       {nullptr};
        VkCommandPool   m_command_pool {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanCommandBuffer(VkCommandBuffer in_handle, VkCommandPool in_command_pool) noexcept;

        VulkanCommandBuffer(VulkanCommandBuffer const& in_copy) = delete;
        VulkanCommandBuffer(VulkanCommandBuffer&&      in_move) noexcept;

        ~VulkanCommandBuffer() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return True if recording could be started, else False.
         * \note   Commands must not be issued if recording is not started.
         */
        [[nodiscard]]
        RkBool Begin(VkCommandBufferUsageFlags             in_usage_flags,
                      VkCommandBufferInheritanceInfo const* in_inheritance_info = nullptr) const noexcept;

        /**
         * \return True if recording could be finished, else False.
         * \note   A command buffer must not be submitted to a queue if recording is not finished.
         */
        [[nodiscard]]
        RkBool End() const noexcept;

        /**
         * \brief Opens a command buffer debug label region.
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        RkVoid BeginLabel(std::string_view in_label_name, Vector4f const& in_color) const noexcept;

        /**
         * \brief Inserts a label into a command buffer.
         * \param in_label_name The name of the label.
         * \param in_color      The RGBA color value that can be associated with the label (ranged from 0.0 to 1.0).
         */
        RkVoid InsertLabel(std::string_view in_label_name, Vector4f const& in_color) const noexcept;

        /**
         * \brief Closes a command buffer label region.
         */
        RkVoid EndLabel() const noexcept;

        /**
         * \brief Inserts a global memory dependency.
         */
        RkVoid InsertMemoryBarrier(VkPipelineStageFlags   in_src_stage,
                                    VkPipelineStageFlags   in_dst_stage,
                                    VkDependencyFlags      in_dependency_flags,
                                    VkMemoryBarrier const& in_memory_barrier) const noexcept;

        /**
         * \brief Inserts a buffer memory dependency.
         */
        RkVoid InsertMemoryBarrier(VkPipelineStageFlags         in_src_stage,
                                    VkPipelineStageFlags         in_dst_stage,
                                    VkDependencyFlags            in_dependency_flags,
                                    VkBufferMemoryBarrier const& in_memory_barrier) const noexcept;

        /**
         * \brief Inserts an image memory dependency.
         */
        RkVoid InsertMemoryBarrier(VkPipelineStageFlags        in_src_stage,
                                    VkPipelineStageFlags        in_dst_stage,
                                    VkDependencyFlags           in_dependency_flags,
                                    VkImageMemoryBarrier const& in_memory_barrier) const noexcept;

        /**
         * \brief Copies data between two buffers of the same size.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyBufferToBuffer(VulkanBuffer const& in_src_buffer,
                                   VulkanBuffer const& in_dst_buffer) const noexcept;

        /**
         * \brief Copies data between two buffer regions.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyBufferToBuffer(VulkanBuffer const& in_src_buffer,
                                   VulkanBuffer const& in_dst_buffer,
                                   VkBufferCopy const& in_region) const noexcept;

        /**
         * \brief Copies data between two images of the same size.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyImageToImage(VulkanImage const& in_src_image,
                                 VulkanImage const& in_dst_image) const noexcept;

        /**
         * \brief Copies data between two image regions.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyImageToImage(VulkanImage const& in_src_image,
                                 VulkanImage const& in_dst_image,
                                 VkImageCopy const& in_region) const noexcept;

        /**
         * \brief Copies data from a buffer into an image.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyBufferToImage(VulkanBuffer const& in_buffer,
                                  VulkanImage  const& in_image) const noexcept;

        /**
         * \brief Copies data from a buffer region into an image region.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyBufferToImage(VulkanBuffer      const& in_buffer,
                                  VulkanImage       const& in_image,
                                  VkBufferImageCopy const& in_region) const noexcept;

        /**
         * \brief Copies image data into a buffer.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyImageToBuffer(VulkanImage  const& in_image,
                                  VulkanBuffer const& in_buffer) const noexcept;

        /**
         * \brief Copies data from an image region into a buffer region.
         * \note  This is a transfer operation supported on all queues.
         */
        RkVoid CopyImageToBuffer(VulkanImage       const& in_image,
                                  VulkanBuffer      const& in_buffer,
                                  VkBufferImageCopy const& in_region) const noexcept;

        /**
         * \brief Copies an image into another one, potentially performing format conversion.
         * \note  This is a transfer operation only supported on graphics queues.
         */
        RkVoid BlitImage(VulkanImage const& in_src_image,
                          VulkanImage const& in_dst_image,
                          VkFilter           in_filter) const noexcept;

        /**
         * \brief Copies regions of an image, potentially performing format conversion.
         * \note  This is a transfer operation only supported on graphics queues.
         */
        RkVoid BlitImage(VulkanImage const& in_src_image,
                          VulkanImage const& in_dst_image,
                          VkImageBlit const& in_region,
                          VkFilter           in_filter) const noexcept;

        [[nodiscard]]
        VkCommandBuffer const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanCommandBuffer& operator=(VulkanCommandBuffer const& in_copy) = delete;
        VulkanCommandBuffer& operator=(VulkanCommandBuffer&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE