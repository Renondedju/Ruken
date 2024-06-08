
#pragma once

#include "Build/Namespace.hpp"
#include "Core/Service.hpp"
#include "Meta/Meta.hpp"

BEGIN_RUKEN_NAMESPACE

class Kernel;
class ServiceProvider;

/**
 * \brief The kernel proxy is a simple class allowing access to the main Kernel class
 *        via the ServiceProvider
 */
class KernelProxy final: public Service<KernelProxy>
{
    private:

        #pragma region Members

        Kernel& m_kernel_reference;

        #pragma endregion

    public:

        #pragma region Constructors

        KernelProxy(ServiceProvider& in_service_provider, Kernel& in_kernel) noexcept;

        KernelProxy(KernelProxy const& in_copy) = delete;
        KernelProxy(KernelProxy&&      in_move) = delete;
        ~KernelProxy()                          = default;

        #pragma endregion

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = RUKEN_STRING(KernelProxy);

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the kernel reference
         * \return Kernel reference
         */
        Kernel& GetKernelReference() const noexcept;

        #pragma endregion

        #pragma region Operators
        
        KernelProxy& operator=(KernelProxy const& in_copy) = delete;
        KernelProxy& operator=(KernelProxy&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE