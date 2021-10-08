#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;
class RenderFrame;

class RenderPass
{
    protected:

        Logger*       m_logger;
        RenderDevice* m_device;

        vk::RenderPass m_handle;

    public:

        #pragma region Operators

        RenderPass(Logger* in_logger, RenderDevice* in_device) noexcept;

        RenderPass(RenderPass const& in_copy) = delete;
        RenderPass(RenderPass&&      in_move) = delete;

        virtual ~RenderPass() noexcept;

        #pragma endregion

        #pragma region Methods

        virtual RkVoid Begin(vk::CommandBuffer const& in_command_buffer, RenderFrame& in_frame) const noexcept;
        virtual RkVoid End  (vk::CommandBuffer const& in_command_buffer) const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderPass& operator=(RenderPass const& in_copy) = delete;
        RenderPass& operator=(RenderPass&&      in_move) = delete;

        #pragma endregion
};

template <typename TType>
concept RenderPassType = std::is_base_of_v<RenderPass, TType>;

END_RUKEN_NAMESPACE