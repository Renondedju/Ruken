#pragma once

#include "../Debug/Assert.hpp"
#include "Core/ServiceProvider.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TService, typename... TArgs>
    requires std::is_constructible_v<TService, ServiceProvider&, TArgs...>
TService* ServiceProvider::ProvideService(TArgs&&... in_args)
    noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>)
{
    TService* new_service {new TService(*this, std::forward<TArgs>(in_args)...)};

    std::type_index const service_id {new_service->GetServiceID()};

    m_services           [service_id] = new_service;
    m_services_order.push(service_id);

    return new_service;
}

template <typename TService>
TService* ServiceProvider::LocateService() noexcept
{
    // Locating the service
    auto const it = m_services.find(typeid(TService));

    // If the service hasn't been found
    if (it == m_services.end())
        return LocateServiceParent<TService>();

    // Otherwise returning the service address 
    return reinterpret_cast<TService*>(it->second);
}

template<typename TService>
TService* ServiceProvider::LocateServiceParent() noexcept
{
    return m_parent ? m_parent->LocateService<TService>() : nullptr;
}

END_RUKEN_NAMESPACE
