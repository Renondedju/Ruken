
#include "Windowing/WindowManager.hpp"

#include "Core/Kernel.hpp"
#include "Core/KernelProxy.hpp"

#include "Debug/Logging/Logger.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

WindowManager::WindowManager(ServiceProvider& in_service_provider) noexcept:
    Service {in_service_provider}
{
    if (auto* root_logger = in_service_provider.LocateService<Logger>())
        m_logger = root_logger->AddChild("Windowing");

    // TODO : SetErrorCallback

    if (!glfwInit())
    {
        if (m_logger)
            m_logger->Fatal("Failed to initialize GLFW!");

        m_service_provider.LocateService<KernelProxy>()->GetKernelReference().RequestShutdown(1);

        return;
    }

    if (m_logger)
        m_logger->Info("GLFW initialized.");

    SetupScreens();

    // TODO : SetMonitorCallback
}

WindowManager::~WindowManager() noexcept
{
    m_windows.clear();
    m_screens.clear();

    glfwTerminate();

    if (m_logger)
        m_logger->Info("GLFW terminated.");
}

#pragma endregion

#pragma region Methods

RkVoid WindowManager::SetupScreens() noexcept
{
    auto        count    = 0;
    auto* const monitors = glfwGetMonitors(&count);

    m_screens.reserve(count);

    for (auto i = 0; i < count; ++i)
    {
        Screen screen(m_logger, monitors[i]);

        m_screens.emplace_back(std::move(screen));
    }
}

RkVoid WindowManager::Update() const noexcept
{
    glfwPollEvents();
}

Window& WindowManager::CreateWindow(WindowParams const& in_params) noexcept
{
    Window window(in_params, m_logger);

    auto& new_window = m_windows.emplace_back(std::move(window));

    on_window_created.Invoke(new_window);

    return new_window;
}

RkVoid WindowManager::DestroyWindow(Window const& in_window) noexcept
{
    auto const it = std::ranges::find(std::as_const(m_windows), in_window);

    if (it != m_windows.cend())
        m_windows.erase(it);
}

#pragma endregion