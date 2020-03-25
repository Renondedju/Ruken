/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

/* TODO Needs to be removed when Kernel is done TODO */

DAEMON_NAMESPACE::WindowManager* DAEMON_NAMESPACE::GWindowManager = nullptr;

/* TODO Needs to be removed when Kernel is done TODO */

USING_DAEMON_NAMESPACE

DAEvoid WindowManager::ErrorCallback(DAEint32 const  in_error_code,
                                     DAEchar  const* in_description)
{
    std::string message;

    switch (in_error_code)
    {
    #define RESULT(res) case GLFW_##res: message = #res; break
            RESULT(NO_ERROR);
            RESULT(NOT_INITIALIZED);
            RESULT(NO_CURRENT_CONTEXT);
            RESULT(INVALID_ENUM);
            RESULT(INVALID_VALUE);
            RESULT(OUT_OF_MEMORY);
            RESULT(API_UNAVAILABLE);
            RESULT(VERSION_UNAVAILABLE);
            RESULT(PLATFORM_ERROR);
            RESULT(FORMAT_UNAVAILABLE);
            RESULT(NO_WINDOW_CONTEXT);
    #undef  RESULT
    
        default:
            message = "GLFW_UNKNOWN_ERROR_CODE";
            break;
    }

    // GWindowManager->m_logger->Error(message + " : " + in_description);
}

DAEvoid WindowManager::MonitorCallback(GLFWmonitor*   in_monitor,
                                       DAEint32 const in_event)
{
    if (in_event == GLFW_CONNECTED)
    {
        // GWindowManager->m_logger->Error(std::string("Screen connected : ") + glfwGetMonitorName(in_monitor));

        GWindowManager->AddScreen(in_monitor);
    }

    else if (in_event == GLFW_DISCONNECTED)
    {
        // GWindowManager->m_logger->Error(std::string("Screen disconnected : ") + glfwGetMonitorName(in_monitor));

        GWindowManager->RemoveScreen(in_monitor);
    }
}

DAEvoid WindowManager::JoystickCallback(DAEint32 const in_jid,
                                        DAEint32 const in_event) noexcept
{
    if (in_event == GLFW_CONNECTED)
    {
        // GWindowManager->m_logger->Error(std::string("New joystick connected : ") + glfwGetJoystickName(in_jid));
    }

    else if (in_event == GLFW_DISCONNECTED)
    {
        // GWindowManager->m_logger->Error(std::string("Joystick disconnected : ") + glfwGetJoystickName(in_jid));
    }
}

DAEvoid WindowManager::AddScreen(GLFWmonitor* in_monitor)
{
    m_screens.emplace_back(Screen(in_monitor));
}

DAEvoid WindowManager::RemoveScreen(GLFWmonitor* in_monitor) noexcept
{
    for (auto it = m_screens.cbegin(); it != m_screens.cend(); ++it)
    {
        if (it->m_handle == in_monitor)
        {
            m_screens.erase(it);

            return;
        }
    }
}

DAEvoid WindowManager::DiscoverScreens()
{
    DAEint32 count;

    GLFWmonitor** monitors = glfwGetMonitors(&count);

    m_screens.reserve(count);

    for (DAEint32 i = 0; i < count; ++i)
    {
        AddScreen(monitors[i]);
    }

    // m_logger->Info("Available screen count : " + std::to_string(count));
}

WindowManager::~WindowManager() noexcept
{
    glfwTerminate();

    // m_logger->Info("GLFW terminated");
}

DAEbool WindowManager::Initialize()
{
    if (glfwInit())
    {
        // Setup callbacks.
        glfwSetErrorCallback   (&ErrorCallback);
        glfwSetMonitorCallback (&MonitorCallback);
        glfwSetJoystickCallback(&JoystickCallback);

        // m_logger->Info("GLFW initialized");

        // Setup screens.
        DiscoverScreens();

        /* TODO Needs to be removed when Kernel is done TODO */

        GWindowManager = this;

        /* TODO Needs to be removed when Kernel is done TODO */

        return true;
    }

    m_logger->Fatal("Failed to initialize GLFW");

    return false;
}

DAEvoid WindowManager::Update() noexcept
{
    glfwPollEvents();
}

Window* WindowManager::CreateWindow(WindowParameters&& in_parameters)
{
    m_windows.emplace_back(Window()).Initialize(std::move(in_parameters));

    return &m_windows.back();
}

DAEbool WindowManager::DestroyWindow(Window* in_window) noexcept
{
    for (auto it = m_windows.cbegin(); it != m_windows.cend(); ++it)
    {
        if (it->m_handle == in_window->m_handle)
        {
            m_windows.erase(it);

            return true;
        }
    }

    return false;
}

Window* WindowManager::GetMainWindow() noexcept
{
    /** TODO Temporarily return the first window TODO */
    return m_windows.empty() ? nullptr : &m_windows.front();
}

Logger* WindowManager::GetLogger() const noexcept
{
    return m_logger;
}

std::vector<Window> const& WindowManager::GetWindows() const noexcept
{
    return m_windows;
}

std::vector<Screen> const& WindowManager::GetScreens() const noexcept
{
    return m_screens;
}