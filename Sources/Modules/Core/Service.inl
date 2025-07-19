#pragma once

template<typename TService>
TService* Service::GetParent(this TService const& in_self) noexcept
{
    return in_self.m_service_provider.template LocateServiceParent<TService>();
}