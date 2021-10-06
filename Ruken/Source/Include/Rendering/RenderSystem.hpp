#pragma once

#include "Rendering/RenderFrame.hpp"

BEGIN_RUKEN_NAMESPACE

class Window;
class RenderDevice;
class ServiceProvider;

class RenderSystem
{
    private:

        RenderDevice* m_device;
        Window* m_window;

        RkUint32 m_current_frame {0U};

        std::vector<std::unique_ptr<RenderFrame>> m_frames;

    public:

        RenderSystem(ServiceProvider& in_service_provider) noexcept;

        RenderSystem(RenderSystem const& in_copy) = delete;
        RenderSystem(RenderSystem&&      in_move) = delete;

        ~RenderSystem() noexcept;

        RkVoid Update() noexcept;

        #pragma region Operators

        RenderSystem& operator=(RenderSystem const& in_copy) = delete;
        RenderSystem& operator=(RenderSystem&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE