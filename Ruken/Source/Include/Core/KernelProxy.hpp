/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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