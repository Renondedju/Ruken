
#include <iostream>

#include "Build/ProjectInfo.hpp"
#include "Build/BuildInfo.hpp"

#include "Core/Kernel.hpp"
#include "Core/KernelProxy.hpp"

#include "Meta/Meta.hpp"
#include "Meta/Safety.hpp"

#include "Rendering/Renderer.hpp"
#include "Threading/Scheduler.hpp"
#include "Windowing/WindowManager.hpp"
#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

Kernel::Kernel()
{
    std::string failure_reason;
    m_logger = m_service_provider.ProvideService<Logger>(failure_reason, "Ruken", ELogLevel::Debug);

    if (m_logger)
    {
        m_logger->AddHandler(&m_console_handler);
        m_logger->Info(std::format("Booting up {} {} rev {} ({} {} > {})", RUKEN_PROJECT_NAME, BuildInfo::Version, BuildInfo::Revision, BuildInfo::SystemName, BuildInfo::BuildType, BuildInfo::CompilerName));
        m_logger->Info(std::format("{} - {} ({})", ProjectInfo::Copyright, ProjectInfo::Licence, ProjectInfo::RepositoryUrl));
    }
    else
    {
        std::cerr << "The logging service failed to initialize for the following reason: " << failure_reason
                  << "\nBecause of that, logging will be unavailable for this session."    << std::endl;
    }

    SetupService<KernelProxy>(true, *this);

    SetupService<Scheduler>      (true);
    SetupService<WindowManager>  (true);
    SetupService<Renderer>       (true);
    SetupService<ResourceManager>(true);

    m_console_handler.Flush();
}

Kernel::~Kernel()
{
    // Flushing the console handler one last time to make sure every log has been displayed
    m_console_handler.Flush();
}

RkInt Kernel::Run() noexcept
{
    // If some required service failed to be initialized
    // killing the kernel before anything else goes wrong
    if (m_exit_code != 0)
        return m_exit_code;

    auto& window_manager = *m_service_provider.LocateService<WindowManager>();
    auto& window         = window_manager.CreateWindow({
        .name = RUKEN_PROJECT_NAME,
        .size = {
            .width  = 1600,
            .height = 900
        },
        .transparent_framebuffer = true,
    });

    // Main kernel loop
    while (!m_shutdown_requested.load(std::memory_order_acquire))
    {
        // Updating services that needs to
        window_manager.Update();

        if (window.ShouldClose())
            RequestShutdown(0);

        // Displaying logs to the console
        m_console_handler.Flush();
    }

    // Exit
    RUKEN_SAFE_LOGGER_CALL(m_logger, Info("Cleanup done, exiting application"))

    return m_exit_code;
}

RkVoid Kernel::RequestShutdown(RkInt const in_exit_code) noexcept
{
    RUKEN_SAFE_LOGGER_CALL(m_logger, Info("Shutdown requested with exit code " + std::to_string(in_exit_code)))

    m_exit_code = in_exit_code;
    m_shutdown_requested.store(true, std::memory_order_release);
}
