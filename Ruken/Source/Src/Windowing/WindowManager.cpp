
#include "Windowing/WindowManager.hpp"

#include "Build/Info.hpp"

#include "Core/Kernel.hpp"
#include "Core/KernelProxy.hpp"

#include "Rendering/Renderer.hpp"

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

    WindowParams params = {
        .name = RUKEN_PROJECT_NAME,
        .size = {
            .width  = 1600U,
            .height = 900U
        },
        .decorated = true,
        .focused   = true,
        .maximized = true
    };

    m_main_window = CreateWindow(params);
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
    RkInt32       count    = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    for (RkInt32 i = 0; i < count; ++i)
    {
        m_screens.emplace_back(new Screen(m_logger, monitors[i]));
    }
}

RkVoid WindowManager::Update() const noexcept
{
    glfwPollEvents();

    if (m_main_window->ShouldClose())
    {
        m_service_provider.LocateService<KernelProxy>()->GetKernelReference().RequestShutdown(1);
    }
}

Window* WindowManager::CreateWindow(WindowParams const& in_params) noexcept
{
    Renderer const* renderer = m_service_provider.LocateService<Renderer>();

    return m_windows.emplace_back(new Window(m_logger, renderer->GetContext(), renderer->GetDevice(), in_params)).get();
}

RkVoid WindowManager::DestroyWindow(Window const& in_window) noexcept
{
    for (auto it = m_windows.cbegin(); it != m_windows.cend(); ++it)
    {
        if ((**it).m_handle != in_window.m_handle)
            continue;

        m_windows.erase(it);

        return;
    }
}

Window* WindowManager::GetMainWindow() const noexcept
{
    return m_main_window;
}

#pragma endregion