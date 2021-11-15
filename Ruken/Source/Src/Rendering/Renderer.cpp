
#include "Rendering/Renderer.hpp"

#include "Core/Kernel.hpp"
#include "Core/KernelProxy.hpp"

#include "Threading/Scheduler.hpp"

#include "Windowing/WindowManager.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Renderer::Renderer(ServiceProvider& in_service_provider) noexcept:
    Service<Renderer> {in_service_provider},
    m_scheduler       {in_service_provider.LocateService<Scheduler>()}
{
    auto& kernel         = in_service_provider.LocateService<KernelProxy>  ()->GetKernelReference();
    auto* window_manager = in_service_provider.LocateService<WindowManager>();
    auto* root_logger    = in_service_provider.LocateService<Logger>       ();

    if (root_logger)
        m_logger = root_logger->AddChild("Rendering");

    VulkanDebug::Initialize(m_logger);

    if (!VulkanLoader::Initialize())
    {
        kernel.RequestShutdown(1);
        return;
    }

    if ((m_instance         = std::make_unique<VulkanInstance>       ())                  ->IsValid() &&
        (m_physical_device  = std::make_unique<VulkanPhysicalDevice> ())                  ->IsValid() &&
        (m_device           = std::make_unique<VulkanDevice>         (*m_scheduler,
                                                                      *m_physical_device))->IsValid() &&
        (m_device_allocator = std::make_unique<VulkanDeviceAllocator>(*m_physical_device))->IsValid())
    {
        window_manager->on_window_created += [this](Window& in_window)
        {
            MakeContext(in_window);
        };

        if (m_logger)
            m_logger->Info("Renderer initialized.");
    }

    else
        kernel.RequestShutdown(1);
}

Renderer::~Renderer() noexcept
{
    if (m_device)
        m_device->WaitIdle();

    m_render_contexts .clear();
    m_device_allocator.reset();
    m_device          .reset();
    m_physical_device .reset();
    m_instance        .reset();

    if (m_logger)
        m_logger->Info("Renderer shutdown.");
}

#pragma endregion

#pragma region Methods

RkVoid Renderer::MakeContext(Window& in_window) noexcept
{
    m_render_contexts.emplace_back(*this, *m_scheduler, in_window);
}

VulkanInstance& Renderer::GetInstance() const noexcept
{
    return *m_instance;
}

VulkanPhysicalDevice& Renderer::GetPhysicalDevice() const noexcept
{
    return *m_physical_device;
}

VulkanDevice& Renderer::GetDevice() const noexcept
{
    return *m_device;
}

VulkanDeviceAllocator& Renderer::GetDeviceAllocator() const noexcept
{
    return *m_device_allocator;
}

#pragma endregion