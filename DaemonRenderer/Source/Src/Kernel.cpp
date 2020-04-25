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

#include <iostream>

#include "Kernel.hpp"
#include "Config.hpp"
#include "Build.hpp"

#include "Threading/Scheduler.hpp"
#include "Windowing/WindowManager.hpp"
#include "Resource/ResourceManager.hpp"

USING_DAEMON_NAMESPACE

DAEvoid Kernel::SetupServices() noexcept
{
    m_service_provider.ProvideService<Scheduler>();
    m_service_provider.ProvideService<WindowManager>();
    m_service_provider.ProvideService<ResourceManager>();
}

Kernel::Kernel():
    m_service_provider   {},
    // Setting up the logger before any other services to make sure that others can use it to output
    // anything while initializing if needed
    m_logger             {*m_service_provider.ProvideService<Logger>("root", ELogLevel::Debug, nullptr, true)},
    m_stream_handler     {&m_console_formatter, std::cout},
    m_console_formatter  {},
    m_shutdown_requested {false},
    m_exit_code          {0}
{
    m_logger.AddHandler(&m_stream_handler);

    m_logger.Info("Booting up " DAEMON_BUILD_INFO);
    m_logger.Info(DAEMON_LICENSE_STR " " DAEMON_COPYRIGHT_STR " (" DAEMON_URL ")");

    SetupServices();
}

DAEint Kernel::Run() noexcept
{
    while (!m_shutdown_requested.load(std::memory_order_acquire))
    {
        RequestShutdown(0);
    }

    m_logger.Info("Cleaning up");

    system("pause");

    return m_exit_code;
}

DAEvoid Kernel::RequestShutdown(DAEint const in_exit_code) noexcept
{
    m_exit_code = in_exit_code;

    m_logger.Info("Shutdown requested with exit code " + std::to_string(in_exit_code));

    m_shutdown_requested.store(true, std::memory_order_release);
}
