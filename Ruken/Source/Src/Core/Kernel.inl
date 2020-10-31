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

template <ServiceType TService, typename ... TArgs, std::enable_if_t<std::is_constructible_v<TService, ServiceProvider&, TArgs...>, RkBool>>
RkBool Kernel::SetupService(RkBool const in_required, TArgs&&...in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>)
{
    // If an exit is required
    if (m_exit_code != 0)
        return false;

    std::string reason  {};
    TService*   service {m_service_provider.ProvideService<TService>(reason, std::forward<TArgs>(in_args)...)};

    if (!service)
    {
        if (in_required)
        {
            RUKEN_SAFE_LOGGER_CALL(m_logger, Error("The required \"" + std::string(TService::service_name) + "\" service failed to initialize for the following reason:"))
            RUKEN_SAFE_LOGGER_CALL(m_logger, Error(reason))
            RUKEN_SAFE_LOGGER_CALL(m_logger, Error("Shutting down kernel with error code 1000"))

            m_exit_code = 1000;

            return false;
        }

        RUKEN_SAFE_LOGGER_CALL(m_logger, Warning("The \"" + std::string(TService::service_name) + "\" service will not be available due to the following reason:"))
        RUKEN_SAFE_LOGGER_CALL(m_logger, Warning(reason))

        return false;
    }

    return true;
}
