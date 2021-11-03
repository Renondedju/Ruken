
#pragma once

#include "Meta/Meta.hpp"

#include "Core/Service.hpp"

#include "Rendering/RenderContext.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderGraph.hpp"
#include "Rendering/RenderTextureStreamer.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Window;

class Renderer final : public Service<Renderer>
{
    private:

        #pragma region Members

        Logger* m_logger {nullptr};

        std::unique_ptr<RenderContext>         m_context;
        std::unique_ptr<RenderDevice>          m_device;
        std::unique_ptr<RenderTextureStreamer> m_texture_streamer;
        std::unique_ptr<RenderGraph>           m_graph;

        #pragma endregion

    public:

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = RUKEN_STRING(Renderer);

        #pragma endregion

        #pragma region Constructors

        Renderer(ServiceProvider& in_service_provider) noexcept;

        Renderer(Renderer const& in_copy) = delete;
        Renderer(Renderer&&      in_move) = delete;

        ~Renderer() noexcept override;

        #pragma endregion

        #pragma region Methods

        RenderContext*         GetContext        () const noexcept;
        RenderDevice*          GetDevice         () const noexcept;
        RenderTextureStreamer* GetTextureStreamer() const noexcept;
        RenderGraph*           GetGraph          () const noexcept;

        #pragma endregion

        #pragma region Operators

        Renderer& operator=(Renderer const& in_copy) = delete;
        Renderer& operator=(Renderer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE