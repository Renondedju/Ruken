#pragma once

#include "ECS/Components/ComponentFingerprint.hpp"

#include <functional>

BEGIN_RUKEN_NAMESPACE

/// @brief Query function to check if a handler is runnable on a given archetype/universe pair.
using ComponentQuery = std::function<RkBool(
	ComponentFingerprint const& in_universe,
	ComponentFingerprint const& in_archetype
)>;

END_RUKEN_NAMESPACE