
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief EResourceStatus describes the status of a resource
 * 
 * Pending   => Waiting to be loaded, this state is only temporary
 * Processed => The resource is being un/re/loaded. Any access is forbidden during this state.
 * Loaded    => The resource has been fully loaded and can now be used.
 * Invalid   => The resource has been invalidated, using it in this status might cause crashes.
 *              An invalidation is caused if the resource is tagged for garbage collection (GC)
 *              or if the resource failed to load for any reason.
 */
enum class EResourceStatus : RkUint8
{
    Pending,
    Processed,
    Loaded,
    Invalid
};

END_RUKEN_NAMESPACE