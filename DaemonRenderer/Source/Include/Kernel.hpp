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

#pragma once

#include <atomic>

#include "Config.hpp"

#include "Types/Unique.hpp"

#include "Core/Service.hpp"
#include "Core/ServiceProvider.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/Handlers/StreamHandler.hpp"
#include "Debug/Logging/Formatters/ConsoleFormatter.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief The kernel is the base class of the application,
 *        ruling the lifetime of the application,
 *        as well as owning every other component of the engine
 * \note This class is unique and cannot be copied, or moved.
 */
class Kernel
{
    private:

        #pragma region Members

        ServiceProvider m_service_provider;

        // Logging
        Logger&          m_logger;
        StreamHandler    m_stream_handler;
        ConsoleFormatter m_console_formatter;

        // Lifetime maintenance
        std::atomic<DAEbool> m_shutdown_requested;
        DAEint               m_exit_code;

        #pragma endregion

        #pragma region Methods

        DAEvoid SetupServices  () noexcept;
        DAEvoid DestroyServices() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        Kernel();
        Kernel(Kernel const& in_copy) = delete;
        Kernel(Kernel&&      in_move) = delete;
        ~Kernel()                     = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Starts the main application loop
         * \see RequestShutdown method
         * \return Exit code
         */
        DAEint Run() noexcept;

        /**
         * \brief Requests the application shutdown with a full cleanup
         *
         * \note This method may be called on any thread
         * \note Once a shutdown is planned, that cannot be canceled.
         * \note The shutdown will be performed at the end of the current frame.
         *       If this function is called before the main loop had a chance to start, then the application
         *       will immediately be stopped once the Run method is called.
         *
         * \param in_exit_code Exit code, this is the code that will be passed
         *        to the return statement of the main once everything is done
         */
        DAEvoid RequestShutdown(DAEint in_exit_code) noexcept;

        #pragma endregion

        #pragma region Operators

        Kernel& operator=(Kernel const& in_copy) = delete;
        Kernel& operator=(Kernel&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE