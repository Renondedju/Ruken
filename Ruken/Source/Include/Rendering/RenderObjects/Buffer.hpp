#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class Buffer
{
    protected:

        RenderDevice* m_device {nullptr};

        vk::Buffer           m_handle          {};
        vk::DeviceSize       m_size            {};
        vk::BufferUsageFlags m_usage           {};
        VmaAllocation        m_allocation      {};
        VmaAllocationInfo    m_allocation_info {};

    public:

        #pragma region Constructors

        Buffer(RenderDevice* in_device, vk::DeviceSize in_size, vk::BufferUsageFlags in_usage) noexcept;
        Buffer(RenderDevice* in_device, vk::BufferCreateInfo const& in_buffer_create_info) noexcept;

        Buffer(Buffer const& in_copy) = delete;
        Buffer(Buffer&&      in_move) = delete;

        virtual ~Buffer() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid  Resize(VkDeviceSize in_size) noexcept;
        RkVoid* Map   ()                     const noexcept;
        RkVoid  UnMap ()                     const noexcept;

        vk::Buffer const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        Buffer& operator=(Buffer const& in_copy) = delete;
        Buffer& operator=(Buffer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE