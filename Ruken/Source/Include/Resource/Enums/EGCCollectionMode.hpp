
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This defines how the garbage collector should behaves
 * 
 * Automatic => Default mode. Automatic GC triggering to optimize ram usage.
 * Disabled  => Disabled, no GC can be triggered when this mode is active. This is useful when reloading a scene
 * Manual    => Manual garbage collection. Automatic GC calls are disabled.
 */
enum class EGCCollectionMode : RkInt8
{
    Disabled,
    Automatic,
    Manual
};

END_RUKEN_NAMESPACE