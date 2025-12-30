#pragma once

#include "Core/Types/Concepts/NonVoid.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Exclusive components are a type of component where only one instance of the component exists in the whole ECS
template <NonVoid TData>
struct UniverseComponent
{
    TData data;
};

END_RUKEN_NAMESPACE