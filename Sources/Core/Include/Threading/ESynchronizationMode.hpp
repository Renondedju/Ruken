
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE
    
/**
 * \brief Describes the mode of loading of a resource
 * 
 * Synchronous  => Uses the current thread to load the resource.
 *                 This is useful if the resource is required before doing anything else  
 * Asynchronous => Creates a task to load the resource without locking the current thread.
 *                 Make sure to check if the resource is available before using it in this case.
 */
enum class ESynchronizationMode : RkUint8
{
    Synchronous,
    Asynchronous
};

END_RUKEN_NAMESPACE