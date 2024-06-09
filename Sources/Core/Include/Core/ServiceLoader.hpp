
#pragma once

#include "Build/Namespace.hpp"
#include "Meta/IsInstance.hpp"
#include "Core/ServiceType.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Helper class for loading a service
 * \tparam TService Service type
 * \tparam TRequired Is the service required for the kernel to boot ?
 */
template <ServiceType TService, RkBool TRequired>
struct ServiceLoader
{
    using Service = TService;

    static constexpr RkBool required = TRequired;
};

template <typename TType>
concept ServiceLoaderType = IsInstance<TType, ServiceLoader>::value;

END_RUKEN_NAMESPACE