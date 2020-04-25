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

template <typename TService, typename ... TArgs, std::enable_if_t<std::is_constructible_v<TService, ServiceProvider&, TArgs...>, DAEbool>>
TService* ServiceProvider::ProvideService(TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>)
{
    static_assert(std::is_base_of_v<Service<TService>, TService>, "The provided service should inherit from Service<TService>");

    TService* new_service = new TService(*this, std::forward<TArgs>(in_args)...);
    m_services[TService::ServiceID()] = reinterpret_cast<ServiceBase*>(new_service);

    return new_service;
}

template <typename TService>
TService* ServiceProvider::LocateService() noexcept
{
    static_assert(std::is_base_of_v<Service<TService>, TService>, "The located service should inherit from Service<TService>");

    // Locating the service
    auto it = m_services.find(TService::ServiceID());

    // If the service hasn't been found, returning nullptr
    if (it == m_services.end())
        return nullptr;

    // Otherwise returning the service address 
    return reinterpret_cast<TService*>(it->second);
}
