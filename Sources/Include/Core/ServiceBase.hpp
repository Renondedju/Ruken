
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
        EInitializationStatus CheckInitializationStatus(std::string& out_reason) const noexcept;

        #pragma endregion

        #pragma region Operators
        
        ServiceBase& operator=(ServiceBase const& in_copy) = default;
        ServiceBase& operator=(ServiceBase&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE