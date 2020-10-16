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
TService* ServiceProvider::ProvideService(TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>)
{
    TService* new_service = new TService(*this, std::forward<TArgs>(in_args)...);

    if (new_service->InitializationSucceeded())
    {
        m_services[TService::ServiceID()] = reinterpret_cast<ServiceBase*>(new_service);
        m_services_order.push(TService::ServiceID());
    }
    else
    {
        delete new_service;
    }

    return new_service;
}

template <ServiceType TService>
TService* ServiceProvider::LocateService() noexcept
{
    // Locating the service
    auto it = m_services.find(TService::ServiceID());

    // If the service hasn't been found, returning nullptr
    if (it == m_services.end())
        return nullptr;

    // Otherwise returning the service address 
    return reinterpret_cast<TService*>(it->second);
}
