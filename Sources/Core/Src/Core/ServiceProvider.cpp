#include "Core/ServiceProvider.hpp"
#include "Core/Service.hpp"

USING_RUKEN_NAMESPACE

ServiceProvider::ServiceProvider(std::string&& in_name, ServiceProvider* in_parent) noexcept:
    m_name   (in_parent ? std::string(in_parent->GetName()) + "." + in_name : in_name),
    m_parent (in_parent)
{}

ServiceProvider::~ServiceProvider() noexcept
{
    // Deleting services by order of initialization
    while (!m_services_order.empty())
    {
        delete m_services[m_services_order.top()];
        m_services.erase(m_services_order.top());
        m_services_order.pop();
    }
}

ServiceProvider* ServiceProvider::GetParent() const noexcept
{
    return m_parent;
}

std::string_view ServiceProvider::GetName() const noexcept
{
    return m_name;
}
