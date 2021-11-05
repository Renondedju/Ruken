
#include "Rendering/Renderer.hpp"

#include "Windowing/WindowManager.hpp"

#include "Core/ServiceProvider.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Renderer::Renderer(ServiceProvider& in_service_provider) noexcept: Service<Renderer> {in_service_provider}
{
    if (Logger* root_logger = m_service_provider.LocateService<Logger>())
        m_logger = root_logger->AddChild("Rendering");

    m_context = std::make_unique<RenderContext>(m_logger);
    m_device  = std::make_unique<RenderDevice> (m_logger, m_context.get());
    m_graph   = std::make_unique<RenderGraph>  (m_logger, m_device .get());

    if (!m_context->GetInstance() || !m_device->GetLogicalDevice())
        return;

    RUKEN_SAFE_LOGGER_CALL(m_logger, Info("Renderer initialized."))
}

Renderer::~Renderer() noexcept
{
    if (!m_device->GetLogicalDevice())
        return;

    vk::Result const result = m_device->GetLogicalDevice().waitIdle();

    if (result != vk::Result::eSuccess)
        RUKEN_SAFE_LOGGER_RETURN_CALL(m_logger, Error("Failed to shutdown renderer : " + vk::to_string(result)))

    m_graph  .reset();
    m_device .reset();
    m_context.reset();

    RUKEN_SAFE_LOGGER_CALL(m_logger, Info("Renderer shutdown."))
}

#pragma endregion

#pragma region Methods

RenderContext* Renderer::GetContext() const noexcept
{
    return m_context.get();
}

RenderDevice* Renderer::GetDevice() const noexcept
{
    return m_device.get();
}

RenderGraph* Renderer::GetGraph() const noexcept
{
    return m_graph.get();
}

#pragma endregion