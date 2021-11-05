#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class ServiceProvider;
class Window;
class Renderer;
class RenderFrame;
class RenderGraph;

class RenderSystem
{
    private:

        Renderer* m_renderer;
        Window*   m_window;

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