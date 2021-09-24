
#pragma once

#include "Meta/Meta.hpp"

#include "Core/Service.hpp"

#include "Rendering/RenderContext.hpp"
#include "Rendering/RenderDevice.hpp"
#include "Rendering/RenderWindow.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Window;



class Renderer final : public Service<Renderer>
{
    private:

        #pragma region Members

        Logger* m_logger {nullptr};

        std::unique_ptr<RenderContext> m_context;
        std::unique_ptr<RenderDevice>  m_device;

        std::vector<RenderWindow> m_render_windows;

        #pragma endregion

        #pragma region Methods

        void OnWindowCreated  (Window& in_window);
        void OnWindowDestroyed(Window& in_window);

        #pragma endregion

    public:

        static vk::RenderPass render_pass;
        static vk::Pipeline pipeline;
        static vk::PipelineLayout pipeline_layout;
        static vk::DescriptorSetLayout descriptor_set_layout;
        static vk::CommandPool command_pool;

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

        RkVoid Update();

        RenderContext const* GetContext() const noexcept;
        RenderDevice  const* GetDevice () const noexcept;

        #pragma endregion

        #pragma region Operators

        Renderer& operator=(Renderer const& in_copy) = delete;
        Renderer& operator=(Renderer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE