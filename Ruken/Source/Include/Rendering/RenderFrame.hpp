#pragma once

#include "Rendering/RenderTarget.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;

class RenderFrame
{
    private:

        #pragma region Members

        Logger*       m_logger {nullptr};
        RenderDevice* m_device {nullptr};

        std::unique_ptr<RenderTarget>    m_color_target         {};
        std::unique_ptr<RenderTarget>    m_depth_target         {};
        vk::Framebuffer m_framebuffer          {};
        RkUint64        m_semaphore_value      {0ULL};
        vk::Semaphore   m_timeline_semaphore   {};
        vk::Semaphore   m_image_semaphore      {};
        vk::Semaphore   m_present_semaphore    {};
        vk::CommandPool m_command_pool         {};
        RkUint32        m_command_buffer_index {};

        std::vector<vk::CommandBuffer> m_command_buffers;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderFrame(Logger* in_logger, RenderDevice* in_device) noexcept;

        RenderFrame(RenderFrame const& in_copy) = delete;
        RenderFrame(RenderFrame&&      in_move) = delete;

        ~RenderFrame() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid                   Reset                          () noexcept;
        RkUint64                 IncrementTimelineSemaphoreValue() noexcept;
        vk::CommandBuffer const& RequestCommandBuffer           () noexcept;

        RenderTarget    const& GetColorTarget           () const noexcept;
        RenderTarget    const& GetDepthTarget           () const noexcept;
        vk::Framebuffer const& GetFramebuffer           () const noexcept;
        RkUint64        const& GetTimelineSemaphoreValue() const noexcept;
        vk::Semaphore   const& GetTimelineSemaphore     () const noexcept;
        vk::Semaphore   const& GetImageSemaphore        () const noexcept;
        vk::Semaphore   const& GetPresentSemaphore      () const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderFrame& operator=(RenderFrame const& in_copy) = delete;
        RenderFrame& operator=(RenderFrame&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE
