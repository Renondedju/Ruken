
#include "Core/ServiceProvider.hpp"

USING_RUKEN_NAMESPACE

ServiceProvider::~ServiceProvider() noexcept
{
    // Deleting services by order of initialization
    while (!m_services_order.empty())
    {
        delete m_services[m_services_order.top()];
        m_services.erase (m_services_order.top());

        m_services_order.pop();
    }
}
