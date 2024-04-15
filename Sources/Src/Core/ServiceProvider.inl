
template <ServiceType TService, typename ... TArgs, std::enable_if_t<std::is_constructible_v<TService, ServiceProvider&, TArgs...>, RkBool>>
TService* ServiceProvider::ProvideService(std::string& out_failure_reason, TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>)
{
    TService* new_service = new TService(*this, std::forward<TArgs>(in_args)...);

    if (new_service->CheckInitializationStatus(out_failure_reason) == EInitializationStatus::Succeeded)
    {
        m_services[TService::ServiceID()] = reinterpret_cast<ServiceBase*>(new_service);
        m_services_order.push(TService::ServiceID());
    }
    else
    {
        delete new_service;
        new_service = nullptr;
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
