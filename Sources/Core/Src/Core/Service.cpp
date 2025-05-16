#include "Core/Service.hpp"

USING_RUKEN_NAMESPACE

Service::Service(ServiceProvider& in_service_provider) noexcept:
	m_service_provider {in_service_provider}
{}