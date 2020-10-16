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

#include "Build/Info.hpp"
#include "Build/Build.hpp"
#include "Build/Config.hpp"

#include "Core/Kernel.hpp"
#include "Core/KernelProxy.hpp"

#include "Meta/Meta.hpp"
#include "Meta/Safety.hpp"

#include "Rendering/Renderer.hpp"
#include "Threading/Scheduler.hpp"
#include "Windowing/WindowManager.hpp"
#include "Resource/ResourceManager.hpp"

USING_RUKEN_NAMESPACE

/**
 * \brief Simple boilerplate code helper to initialize a service
 * \param in_service_type Type name of the service
 * \param in_required Whether or not the service is required to run
 */
#define INIT_SERVICE(in_service_type, in_required, ...)\
    {\
        in_service_type* service {m_service_provider.ProvideService<in_service_type>(__VA_ARGS__)};\
        std::string_view reason  {};\
\
        if (service->CheckInitializationStatus(reason) == EInitializationStatus::Failed)\
        {\
            if (in_required)\
            {\
                RUKEN_SAFE_LOGGER_CALL(m_logger, Error("The required \"" RUKEN_STRING(in_service_type) "\" service failed to initialize for the following reason:"))\
                RUKEN_SAFE_LOGGER_CALL(m_logger, Error(reason))\
                RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Shutting down kernel with error code 1000"))\
                m_exit_code = 1000;\
                return;\
            }\
            else\
            {\
                RUKEN_SAFE_LOGGER_CALL(m_logger, Warning("The \"" RUKEN_STRING(in_service_type) "\" service will not be available due to the following reason:"))\
                RUKEN_SAFE_LOGGER_CALL(m_logger, Warning(reason))\
            }\
        }\
    }

Kernel::Kernel()
{
    #if defined(RUKEN_LOGGING_ENABLED)

        m_logger = m_service_provider.ProvideService<Logger>("Root", ELogLevel::Debug);

        m_logger->AddHandler(&m_console_handler);
        m_logger->Info("Booting up " RUKEN_BUILD_INFO);
        m_logger->Info(RUKEN_LICENSE_STR " " RUKEN_COPYRIGHT_STR " (" RUKEN_URL ")");

    #endif

    INIT_SERVICE(KernelProxy, true, *this)

    INIT_SERVICE(Scheduler      , true)
    INIT_SERVICE(ResourceManager, true)
    INIT_SERVICE(WindowManager  , true)
    INIT_SERVICE(Renderer       , true)

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
        }
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
