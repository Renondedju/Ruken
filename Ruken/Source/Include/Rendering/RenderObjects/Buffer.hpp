#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class Buffer
{
    protected:

        RenderDevice* m_device {nullptr};

        vk::Buffer         m_handle          {};
        vk::Allocation     m_allocation      {};
        vk::AllocationInfo m_allocation_info {};

    public:

        #pragma region Constructors

        Buffer(RenderDevice* in_device, vk::DeviceSize in_size, vk::BufferUsageFlags in_usage) noexcept;

        Buffer(Buffer const& in_copy) = delete;
        Buffer(Buffer&&      in_move) = delete;

        virtual ~Buffer() noexcept;

        #pragma endregion

        #pragma region Methods

        vk::Buffer const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        Buffer& operator=(Buffer const& in_copy) = delete;
        Buffer& operator=(Buffer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE