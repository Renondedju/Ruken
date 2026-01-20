#pragma once

#include "Core/Types/Concepts/NonVoid.hpp"
#include "Core/Meta/IsBaseOfTemplate.hpp"
#include "Core/JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "ECS/Components/Component.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Exclusive components are a type of component where only one instance of the component exists in the whole ECS
template <NonVoid TData>
struct UniverseComponent: Component
{
    SharedMutex<TData> data;
};

template <typename TType>
struct UniverseComponentType: IsBaseOfTemplate<UniverseComponent, TType>{};

template <typename TType>
concept IsUniverseComponent = UniverseComponentType<TType>::value;

END_RUKEN_NAMESPACE