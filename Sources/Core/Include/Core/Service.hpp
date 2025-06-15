#pragma once

#include "Core/ServiceProvider.hpp"

#include <string_view>

BEGIN_RUKEN_NAMESPACE

/// @brief Service base class. Must be inherited from to implement a service.
struct Service
{
    #pragma region Lifetime

	// FIXME: Services can be instantiated outside of a provider. This could be a source of error.

    /**
     * @brief Default constructor
     * @param in_service_provider Service provider instance
     * @warning Any service inheriting from this class must declare a default constructor that
     *          takes in the service provider instance as the FIRST parameter.
     */
    explicit Service(ServiceProvider& in_service_provider) noexcept;
    Service           (Service const& in_copy) = delete;
    Service           (Service&&      in_move) = delete;
    Service& operator=(Service const& in_copy) = delete;
    Service& operator=(Service&&      in_move) = delete;
    virtual ~Service()                         = default;

    #pragma endregion

    // Static name of the service, used by the kernel to report service errors
    constexpr static std::string_view service_name = "Unknown";

    /// @return Parent service if any
    template <typename TService>
    TService* GetParent(this TService const& in_self) noexcept;

	protected:

		ServiceProvider& m_service_provider;
};

#include "Core/Service.inl"

END_RUKEN_NAMESPACE