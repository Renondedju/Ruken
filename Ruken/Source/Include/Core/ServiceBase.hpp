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

#include <string>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Core/EInitializationStatus.hpp"

BEGIN_RUKEN_NAMESPACE

class ServiceBase
{
    private:

        #pragma region Members

        EInitializationStatus m_status         {EInitializationStatus::Succeeded};
        std::string           m_failure_reason {""};

        #pragma endregion

    protected:

        #pragma region Methods

        /**
         * \brief Returns the next service ID
         * \return Service ID
         */
        static RkSize GetNextId() noexcept;

        /**
         * \brief Signals a service initialization failure.
         *        This will allow the Kernel to safely stop any other initialization if that service was required and report the passed reason.
         *        If the service was required, this will cause the kernel to shutdown.
         *
         * \param in_reason Reason of the failure
         */
        RkVoid SignalServiceInitializationFailure(std::string&& in_reason) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        ServiceBase()                           = default;
        ServiceBase(ServiceBase const& in_copy) = default;
        ServiceBase(ServiceBase&&      in_move) = default;
        virtual ~ServiceBase()                  = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks the initialization status of the service
         * \param out_reason Reason of the failure if a failure occurred
         * \return Service status, will be EInitializationStatus::Succeeded if everything is fine, EInitializationStatus::Failed otherwise
         */
        [[nodiscard]]
        EInitializationStatus CheckInitializationStatus(std::string_view& out_reason) const noexcept;

        /**
         * \brief Checks if the service is initialized and can be used
         *        If the service is not available, please use the CheckInitializationStatus method for more detailed information
         *
         * \return True if the service is available, false otherwise
         */
        [[nodiscard]]
        RkBool InitializationSucceeded() const noexcept;

        #pragma endregion

        #pragma region Operators
        
        ServiceBase& operator=(ServiceBase const& in_copy) = default;
        ServiceBase& operator=(ServiceBase&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE