#pragma once

#include "Build/Namespace.hpp"

#include <stack>
#include <string>
#include <typeindex>
#include <unordered_map>

BEGIN_RUKEN_NAMESPACE

struct Service;

/// @brief Locates and provides services using a tree-like structure.
class ServiceProvider
{
    #pragma region Members

    // Provided services key = service id, value = service pointer
	std::string									  m_name;
	ServiceProvider*							  m_parent;
    std::unordered_map<std::type_index, Service*> m_services	   {};
    std::stack        <std::type_index>			  m_services_order {};

    #pragma endregion

    public:

        #pragma region Lifetime

	    explicit ServiceProvider(std::string&& in_name, ServiceProvider* in_parent = nullptr) noexcept;
		ServiceProvider& operator=(ServiceProvider const& in_copy) = default;
		ServiceProvider& operator=(ServiceProvider&&      in_move) = default;
        ServiceProvider			  (ServiceProvider const& in_copy) = default;
        ServiceProvider			  (ServiceProvider&&      in_move) = default;
        ~ServiceProvider() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Provides and holds a service to allow others parts of the code to locate it later on if needed
         *
         * \tparam TService Service type, must inherit from the Service class
         * \tparam TArgs TService constructor types, excluding the service provider instance type
         * \param in_args Arguments to pass to the TService constructor, excluding the service provider instance.
         * \warning Providing a service that has already been provided will override the previous instance.
         * \return New service instance.
         */
        template <typename TService, typename... TArgs>
			requires std::is_constructible_v<TService, ServiceProvider&, TArgs...>
        TService* ProvideService(TArgs&&... in_args) noexcept(std::is_nothrow_constructible_v<TService, ServiceProvider&, TArgs...>);

        /**
         * @brief Locates a service. This service could be unavailable or unprovided yet, if this is the case, a nullptr will be returned
         * @tparam TService Service type, must inherit from the Service class.
         * @return Located service, could be null if the service is unavailable or unprovided.
         */
        template <typename TService>
        [[nodiscard]] TService* LocateService() const noexcept;

	    /**
	     * Similar to LocateService() but only checks in parent providers.
		 * @tparam TService Service type, must inherit from the Service class.
		 * @return Located service, could be null if the service is unavailable or unprovided.
	     */
	    template <typename TService>
		[[nodiscard]] TService* LocateServiceParent() const noexcept;

		/// @brief Returns the parent locator
		[[nodiscard]] ServiceProvider* GetParent() const noexcept;

		/// @brief Returns the name of the locator
		[[nodiscard]] std::string_view GetName() const noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE

#include "ServiceProvider.inl"
