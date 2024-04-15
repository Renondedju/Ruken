
#include "Core/KernelProxy.hpp"

USING_RUKEN_NAMESPACE

KernelProxy::KernelProxy(ServiceProvider& in_service_provider, Kernel& in_kernel) noexcept:
    Service<KernelProxy> {in_service_provider},
    m_kernel_reference   {in_kernel}
{}

Kernel& KernelProxy::GetKernelReference() const noexcept
{
    return m_kernel_reference;
}
