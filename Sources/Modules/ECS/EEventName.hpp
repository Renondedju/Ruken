#pragma once

#include "Bitwise/Bitmask.hpp"
#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Enumerates all types of events that could be fired in the ECS
enum class EECSEventName
{
    OnStart ,
    OnUpdate,
    OnEnd   ,
};

using EECSEvents = Bitmask<EECSEventName>;

END_RUKEN_NAMESPACE