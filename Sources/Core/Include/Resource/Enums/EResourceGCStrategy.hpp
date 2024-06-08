
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief EResourceGCStrategy describes the Garbage Collection (GC) strategy of a resource
 * 
 * ReferenceCount => Default strategy : deletes the resource when there is no usage left on it
 * KeepEverywhere => Deletion will occur once the game has been shutdown.
 * SceneDeletion  => Deletion will occur when the current scene is deleted.
 * Manual         => Manual GC, this allows the use of the Handle<Resource_Type>::deleteResource() method. 
 *                   Doing so will invalidate every handle on this resource.
 */
enum class EResourceGCStrategy : RkUint8
{
    ReferenceCount,
    KeepEverywhere,
    SceneDeletion,
    Manual
};

END_RUKEN_NAMESPACE