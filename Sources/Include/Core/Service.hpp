
#pragma once

#include "Build/Namespace.hpp"
#include "Core/ServiceBase.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class ServiceProvider;

/**
 * \brief Service base class. Must be inherited from to implement a service.
 * \tparam TCrtp This class uses the Curiously Recurring Template Pattern. This type must be the actual service implementation class.
 */
template <typename TCrtp>
class Service : public ServiceBase
{
    protected:

        #pragma region Members

        ServiceProvider& m_service_provider;

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_service_provider Service provider instance
         * \warning Any service service inheriting from this class must declare a default constructor that
         *          takes in the service provider instance as the FIRST parameter.
         */
        Service(ServiceProvider& in_service_provider) noexcept;

        Service(Service const& in_copy) = delete;
        Service(Service&&      in_move) = delete;
        virtual ~Service()              = default;

        #pragma endregion

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = "Unknown";

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Retrieves the unique ID of the service
         * \return Service ID
         */
        static RkSize ServiceID() noexcept;

        #pragma endregion

        #pragma region Operators
        
        Service& operator=(Service const& in_copy) = delete;
        Service& operator=(Service&&      in_move) = delete;

        #pragma endregion
};

#include "Core/Service.inl"

END_RUKEN_NAMESPACE