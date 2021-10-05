#pragma once

#include "Rendering/RenderFrame.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;
class Renderer;

class RenderSystem
{
    private:

        RenderDevice* m_device;
        Renderer* m_renderer;

        RkUint32 m_current_frame {0U};

        std::vector<std::unique_ptr<RenderFrame>> m_frames;

    public:

        RenderSystem(RenderDevice* in_device, Renderer* in_renderer) noexcept;

        RenderSystem(RenderSystem const& in_copy) = delete;
        RenderSystem(RenderSystem&&      in_move) = delete;

        ~RenderSystem() noexcept;

        RkVoid Render();

        #pragma region Operators

        RenderSystem& operator=(RenderSystem const& in_copy) = delete;
        RenderSystem& operator=(RenderSystem&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE