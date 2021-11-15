
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
