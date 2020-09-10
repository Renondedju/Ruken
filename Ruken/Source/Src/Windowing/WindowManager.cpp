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

#include <GLFW/glfw3.h>

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
    Window window(m_logger, in_params);

    auto& new_window = m_windows.emplace_back(std::move(window));

    on_window_created.Invoke(new_window);

    return new_window;
}

RkVoid WindowManager::DestroyWindow(Window const& in_window) noexcept
{
    auto const it = std::find(m_windows.cbegin(), m_windows.cend(), in_window);

    if (it != m_windows.cend())
        m_windows.erase(it);
}

#pragma endregion