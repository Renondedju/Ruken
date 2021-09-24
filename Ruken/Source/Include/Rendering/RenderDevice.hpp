#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderContext;

class RenderDevice
{
    private:

        #pragma region Members

        Logger* m_logger;

        vk::PhysicalDevice m_physical_device;
        vk::Device         m_logical_device;
        vk::Allocator      m_allocator;

        std::vector<std::vector<vk::Queue>> m_queues;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderDevice(RenderContext const& in_context, Logger* in_logger = nullptr) noexcept;

        RenderDevice(RenderDevice const& in_copy) = delete;
        RenderDevice(RenderDevice&&      in_move) = delete;

        ~RenderDevice() noexcept;

        #pragma endregion

        #pragma region Methods

        vk::PhysicalDevice const& GetPhysicalDevice () const noexcept;
        vk::Device         const& GetLogicalDevice  () const noexcept;
        vk::Allocator      const& GetAllocator      () const noexcept;

        vk::Queue const& GetTestQueue() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderDevice& operator=(RenderDevice const& in_copy) = delete;
        RenderDevice& operator=(RenderDevice&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE