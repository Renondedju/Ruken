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

#include "Build/Namespace.hpp"

// Used in the inlined file
#include "Meta/Meta.hpp"
#include "Meta/Safety.hpp"

#include "Core/Service.hpp"
#include "Core/ServiceProvider.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/Handlers/ConsoleHandler.hpp"

BEGIN_RUKEN_NAMESPACE

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

        ServiceProvider m_service_provider {};

        Logger*             m_logger             {nullptr};
        RkInt               m_exit_code          {0};
        ConsoleHandler      m_console_handler    {};
        std::atomic<RkBool> m_shutdown_requested {false};

        #pragma endregion

    public:

        #pragma region Constructors

        Kernel();
        Kernel(Kernel const& in_copy) = delete;
        Kernel(Kernel&& in_move)      = delete;
        ~Kernel();

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Starts the main application loop
         * \see RequestShutdown method
         * \return Exit code
         */
        RkInt Run() noexcept;

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
        RkVoid RequestShutdown(RkInt in_exit_code) noexcept;

        /**
         * \brief Attempts to setup a service to the service provider
         * \note If a required service fails, any consequent call to this method will be ignored to allow
         *       the kernel to cleanup as fast as possible without generating any more errors
         *
         * \tparam TService Service type to setup
         * \tparam TArgs Constructor argument types of the service
         * \param in_required If set to true, the service will be considered as required and will automatically kill the kernel in case of failure
         * \param in_args Arguments to be forwarded to the service constructor
         * \return True if the service successfully initialized, false otherwise
         */
        template <ServiceType TService, typename... TArgs, std::enable_if_t<std::is_constructible_v<TService, ServiceProvider&, TArgs...>, RkBool> = true>
        RkBool SetupService(RkBool in_required, TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>);

        #pragma endregion

        #pragma region Operators

        Kernel& operator=(Kernel const& in_copy) = delete;
        Kernel& operator=(Kernel&& in_move)      = delete;

        #pragma endregion
};

#include "Core/Kernel.inl"

END_RUKEN_NAMESPACE
