#pragma once

#include "Rendering/RenderQueue.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderContext;

class RenderDevice
{
    private:

        #pragma region Members

        Logger*        m_logger  {nullptr};
        RenderContext* m_context {nullptr};

        vk::PhysicalDevice m_physical_device {};
        vk::Device         m_logical_device  {};
        vk::Allocator      m_allocator       {};

        RkUint32 m_graphics_family_index {UINT32_MAX};
        RkUint32 m_compute_family_index  {UINT32_MAX};
        RkUint32 m_transfer_family_index {UINT32_MAX};

        std::unique_ptr<RenderQueue> m_graphics_queue;
        std::unique_ptr<RenderQueue> m_compute_queue;
        std::unique_ptr<RenderQueue> m_transfer_queue;

        vk::DescriptorPool      m_texture_descriptor_pool;
        vk::DescriptorSetLayout m_texture_descriptor_set_layout;
        vk::DescriptorSet       m_texture_descriptor_set;

        #pragma endregion

        #pragma region Methods

        RkBool PickPhysicalDevice   () noexcept;
        RkVoid FindQueueFamilies    () noexcept;
        RkBool CreateLogicalDevice  () noexcept;
        RkBool CreateDeviceAllocator() noexcept;
        RkVoid CreateTextureDescriptorSet() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderDevice(Logger* in_logger, RenderContext* in_context) noexcept;

        RenderDevice(RenderDevice const& in_copy) = delete;
        RenderDevice(RenderDevice&&      in_move) = delete;

        ~RenderDevice() noexcept;

        #pragma endregion

        #pragma region Methods

        vk::PhysicalDevice      const& GetPhysicalDevice            () const noexcept;
        vk::Device              const& GetLogicalDevice             () const noexcept;
        vk::Allocator           const& GetAllocator                 () const noexcept;
        RkUint32                       GetGraphicsFamilyIndex       () const noexcept;
        RkUint32                       GetComputeFamilyIndex        () const noexcept;
        RkUint32                       GetTransferFamilyIndex       () const noexcept;
        RenderQueue&                   GetGraphicsQueue             () const noexcept;
        RenderQueue&                   GetComputeQueue              () const noexcept;
        RenderQueue&                   GetTransferQueue             () const noexcept;
        RkBool                         HasDedicatedComputeQueue     () const noexcept;
        RkBool                         HasDedicatedTransferQueue    () const noexcept;
        vk::DescriptorSetLayout const& GetTextureDescriptorSetLayout() const noexcept;
        vk::DescriptorSet       const& GetTextureDescriptorSet      () const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderDevice& operator=(RenderDevice const& in_copy) = delete;
        RenderDevice& operator=(RenderDevice&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE