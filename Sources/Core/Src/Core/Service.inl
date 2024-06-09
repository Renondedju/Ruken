
template <typename TCrtp>
Service<TCrtp>::Service(ServiceProvider& in_service_provider) noexcept:
    m_service_provider {in_service_provider}
{}

template <typename TCrtp>
RkSize Service<TCrtp>::ServiceID() noexcept
{
    // Generating the ID once
    static RkSize id = GetNextId();
    
    return id;
}
