#include "Core/Service.hpp"

USING_RUKEN_NAMESPACE

Service::Service(ServiceProvider& in_service_provider, std::type_index const in_service_id) noexcept:
	m_service_provider {in_service_provider},
	m_service_id       {in_service_id}
{}

std::type_index Service::GetServiceID() const noexcept
{
	return m_service_id;
}