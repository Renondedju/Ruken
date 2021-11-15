
#pragma once

#include <stack>
#include <type_traits>
#include <unordered_map>

#include "Build/Namespace.hpp"

#include "Core/Service.hpp"
#include "Core/ServiceBase.hpp"
#include "Core/ServiceType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Locates and provides services
 */
class ServiceProvider
{
    private:

        #pragma region Members

        // Provided services key = service id, value = service pointer
        std::unordered_map<RkSize, ServiceBase*> m_services;
        std::stack<RkSize>                       m_services_order;

        #pragma endregion

    public:

        #pragma region Constructors

        ServiceProvider()                               = default;
        ServiceProvider(ServiceProvider const& in_copy) = default;
        ServiceProvider(ServiceProvider&&      in_move) = default;
        ~ServiceProvider() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Provides and holds a service to allow others parts of the code to locate it later on if needed
         *
         * \tparam TService Service type, must inherit from the Service class
         * \tparam TArgs TService constructor types, excluding the service provider instance type
         * \param in_args Arguments to pass to the TService constructor, excluding the service provider instance
         * \param out_failure_reason Reason of the failure if an error occurred when initializing the service
         * \warning Providing a service that has already been provided
         *          will override the previous instance without destroying it, leading to memory leaks
         * \return New service instance or nullptr if the service failed to be initialized properly
         */
        template <ServiceType TService, typename... TArgs, std::enable_if_t<std::is_constructible_v<TService, ServiceProvider&, TArgs...>, RkBool> = true>
        TService* ProvideService(std::string& out_failure_reason, TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>);

        /**
         * \brief Locates a service. This service could be unavailable or unprovided yet, if this is the case, a nullptr will be returned
         * \tparam TService Service type, must inherit from the Service class
         * \return Located service, could be null if the service is unavailable or unprovided.
         */
        template <ServiceType TService>
        [[nodiscard]] TService* LocateService() noexcept;

        #pragma endregion

        #pragma region Operators
        
        ServiceProvider& operator=(ServiceProvider const& in_copy) = default;
        ServiceProvider& operator=(ServiceProvider&&      in_move) = default;

        #pragma endregion
};

#include "Core/ServiceProvider.inl"

END_RUKEN_NAMESPACE