#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Enumerates all types of events that could be fired in the ECS
 */
enum class EEventName
{
    OnStart,
    OnUpdate,
    OnEnd,
};

END_RUKEN_NAMESPACE