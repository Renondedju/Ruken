#pragma once

#include "Core/ServiceProvider.hpp"
#include "Core/Types/FundamentalTypes.hpp"

#include <string_view>

BEGIN_RUKEN_NAMESPACE

/// @brief Service base class. Must be inherited from to implement a service.
struct Service
{
	/// @brief Pretty name of the service.
	constexpr static std::string_view service_name = "Unknown";

    /// @brief Returns the parent service instance or nullptr.
    template <typename TService>
    TService* GetParent(this TService const& in_self) noexcept;

	/// @brief Type index is stored this way for interface services.
	/// This allows the service provider to properly handle polymorphism.
	std::type_index GetServiceID() const noexcept;

	#pragma region Lifetime

	Service           (Service const& in_copy) = delete;
	Service           (Service&&      in_move) = delete;
	Service& operator=(Service const& in_copy) = delete;
	Service& operator=(Service&&      in_move) = delete;
	virtual ~Service()                         = default;

	#pragma endregion

	protected:

		ServiceProvider&      m_service_provider;
		const std::type_index m_service_id;

		/**
		 * @brief Default constructor.
		 * @param in_service_provider Service provider instance.
		 * @param in_service_id Type index of the child class.
		 *        This allows the service provider to properly handle polymorphic services.
		 * @warning Any service inheriting from this class must declare a default constructor that
		 *          takes in the service provider instance as the FIRST parameter.
		 */
		explicit Service(ServiceProvider& in_service_provider, std::type_index in_service_id) noexcept;
};

#include "Core/Service.inl"

END_RUKEN_NAMESPACE
