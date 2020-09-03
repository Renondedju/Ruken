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

#include "Kernel.hpp"
#include "Config.hpp"
#include "Build.hpp"
#include "KernelProxy.hpp"

#include "Rendering/Renderer.hpp"
#include "Threading/Scheduler.hpp"
#include "Windowing/WindowManager.hpp"
#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

RkVoid Kernel::SetupServices() noexcept
{
    m_service_provider.ProvideService<KernelProxy>(*this);

    m_service_provider.ProvideService<Scheduler>();
    m_service_provider.ProvideService<WindowManager>();
    m_service_provider.ProvideService<ResourceManager>();
    m_service_provider.ProvideService<Renderer>();
}

RkVoid Kernel::DestroyServices() noexcept
{
    m_service_provider.DestroyService<Renderer>();
    m_service_provider.DestroyService<WindowManager>();
    m_service_provider.DestroyService<ResourceManager>();
}

Kernel::Kernel():
    m_service_provider   {},
    // Setting up the logger before any other services to make sure that others can use it to output
    // anything while initializing if needed
    m_logger             {*m_service_provider.ProvideService<Logger>("Root", ELogLevel::Debug)},
    m_shutdown_requested {false},
    m_exit_code          {0}
{
    m_logger.AddHandler(&m_console_handler);

    m_logger.Info("Booting up " RUKEN_BUILD_INFO);
    m_logger.Info(RUKEN_LICENSE_STR " " RUKEN_COPYRIGHT_STR " (" RUKEN_URL ")");

    SetupServices();
}

RkInt Kernel::Run() noexcept
{
    auto& window_manager = *m_service_provider.LocateService<WindowManager>();

    WindowParams params = {};

    params.name        = RUKEN_PROJECT_NAME;
    params.size.width  = 1600;
    params.size.height = 900;

    auto& window = window_manager.CreateWindow(params);

    while (!m_shutdown_requested.load(std::memory_order_acquire))
    {
        window_manager.Update();

        if (window.ShouldClose())
            RequestShutdown(0);

        // TODO : Call this in a separate thread to avoid stalling ?
        m_console_handler.Flush();
    }

    DestroyServices();

    m_logger.Info("Cleanup done, exiting application");

    // TODO : Maybe call this in the destructor ?
    m_console_handler.Flush();

    return m_exit_code;
}

RkVoid Kernel::RequestShutdown(RkInt const in_exit_code) noexcept
{
    m_exit_code = in_exit_code;

    m_logger.Info("Shutdown requested with exit code " + std::to_string(in_exit_code));

    m_shutdown_requested.store(true, std::memory_order_release);
}
