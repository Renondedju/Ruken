/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "Windowing/WindowManager.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

WindowManager::WindowManager():
    m_logger {GRootLogger->AddChild("Windowing")}
{
    /* TODO Needs to be removed when Kernel is done TODO */

    GWindowManager = this;

    /* TODO Needs to be removed when Kernel is done TODO */

    glfwSetErrorCallback(&ErrorCallback);

    if (glfwInit())
    {
        glfwSetMonitorCallback(&MonitorCallback);

        DiscoverScreens();

        m_logger->Info("GLFW initialized successfully.");
    }

    else
        m_logger->Fatal("Failed to initialize GLFW!");
}

WindowManager::~WindowManager() noexcept
{
    m_windows.clear();
    m_screens.clear();

    glfwTerminate();

    m_logger->Info("GLFW terminated.");
}

#pragma endregion

#pragma region Methods

#pragma region Callbacks

DAEvoid WindowManager::ErrorCallback(DAEint32 const in_error_code, DAEchar const*in_description)
{
    std::string message;

    switch (in_error_code)
    {
        case GLFW_NOT_INITIALIZED:
            message = "GLFW_NOT_INITIALIZED";
            break;

        case GLFW_NO_CURRENT_CONTEXT:
            message = "GLFW_NO_CURRENT_CONTEXT";
            break;

        case GLFW_INVALID_ENUM:
            message = "GLFW_INVALID_ENUM";
            break;

        case GLFW_INVALID_VALUE:
            message = "GLFW_INVALID_VALUE";
            break;

        case GLFW_OUT_OF_MEMORY:
            message = "GLFW_OUT_OF_MEMORY";
            break;

        case GLFW_API_UNAVAILABLE:
            message = "GLFW_API_UNAVAILABLE";
            break;

        case GLFW_VERSION_UNAVAILABLE:
            message = "GLFW_VERSION_UNAVAILABLE";
            break;

        case GLFW_PLATFORM_ERROR:
            message = "GLFW_PLATFORM_ERROR";
            break;

        case GLFW_FORMAT_UNAVAILABLE:
            message = "GLFW_FORMAT_UNAVAILABLE";
            break;

        case GLFW_NO_WINDOW_CONTEXT:
            message = "GLFW_NO_WINDOW_CONTEXT";
            break;

        default:
            message = "GLFW_UNKNOWN_ERROR_CODE";
            break;
    }

    GWindowManager->m_logger->Error(message + " : " + in_description);
}

DAEvoid WindowManager::MonitorCallback(GLFWmonitor* in_monitor, DAEint32 const in_event)
{
    if (in_event == GLFW_CONNECTED)
    {
        GWindowManager->AddScreen(in_monitor);

        GWindowManager->m_logger->Error(std::string("Screen disconnected : ").append(glfwGetMonitorName(in_monitor)));
    }

    else if (in_event == GLFW_DISCONNECTED)
    {
        GWindowManager->RemoveScreen(in_monitor);

        GWindowManager->m_logger->Error(std::string("Screen disconnected : ").append(glfwGetMonitorName(in_monitor)));
    }
}

#pragma endregion

DAEvoid WindowManager::AddScreen(GLFWmonitor* in_monitor)
{
    auto& screen = m_screens.emplace_back(std::make_unique<Screen>(in_monitor));

    on_screen_created.Invoke(*screen);
}

DAEvoid WindowManager::RemoveScreen(GLFWmonitor* in_monitor) noexcept
{
    for (auto it = m_screens.begin(); it != m_screens.end(); ++it)
    {
        if (it->get()->GetHandle() == in_monitor)
        {
            on_screen_destroyed.Invoke(*it->get());

            m_screens.erase(it);

            return;
        }
    }
}

DAEvoid WindowManager::DiscoverScreens()
{
    DAEint32 count;

    auto const monitors = glfwGetMonitors(&count);

    m_screens.reserve(count);

    for (auto i = 0; i < count; ++i)
    {
        AddScreen(monitors[i]);
    }

    m_logger->Info("Available screen count : " + std::to_string(count));
}

DAEvoid WindowManager::Update() noexcept
{
    glfwPollEvents();
}

Window& WindowManager::CreateWindow(WindowParams const& in_params)
{
    auto& window = m_windows.emplace_back(std::make_unique<Window>(in_params));

    on_window_created.Invoke(*window);

    return *window;
}

DAEbool WindowManager::DestroyWindow(Window const* in_window) noexcept
{
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it)
    {
        if (it->get()->GetHandle() == in_window->m_handle)
        {
            on_window_destroyed.Invoke(*it->get());

            m_windows.erase(it);

            return true;
        }
    }

    return false;
}

Logger* WindowManager::GetLogger() const noexcept
{
    return m_logger;
}

Window& WindowManager::GetWindow(DAEuint32 const in_index) noexcept
{
    return *m_windows[in_index];
}

Window& WindowManager::GetMainWindow() noexcept
{
    return *m_windows.front();
}

Screen& WindowManager::GetScreen(DAEuint32 const in_index) noexcept
{
    return *m_screens[in_index];
}

#pragma endregion