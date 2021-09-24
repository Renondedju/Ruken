#pragma once

#include "Rendering/Resources/Texture.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;

class RenderFrame
{
    friend class RenderWindow;

    private:

        #pragma region Members

        Logger*       m_logger;
        RenderDevice* m_device;

        vk::CommandBuffer m_command_buffer;
        vk::Fence         m_fence;
        vk::Semaphore     m_image_semaphore;
        vk::Semaphore     m_present_semaphore;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderFrame(RenderDevice* in_device, Logger* in_logger = nullptr) noexcept;

        RenderFrame(RenderFrame const& in_copy) = delete;
        RenderFrame(RenderFrame&&      in_move) = default;

        ~RenderFrame() noexcept;

        #pragma endregion

        #pragma region Methods



        #pragma endregion

        #pragma region Operators

        RenderFrame& operator=(RenderFrame const& in_copy) = delete;
        RenderFrame& operator=(RenderFrame&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE
