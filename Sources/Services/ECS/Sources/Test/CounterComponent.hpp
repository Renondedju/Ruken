
#pragma once

#include <array>

#include "ECS/Component.hpp"
#include "ECS/TagComponent.hpp"
#include "ECS/ExclusiveComponent.hpp"

USING_RUKEN_NAMESPACE

// Creating a component
RUKEN_DECLARE_COMPONENT(CounterComponent,
    RUKEN_DECLARE_FIELD(CountField, RkSize),
    RUKEN_DECLARE_FIELD(TestPaddingField, std::array<RkSize, 10>)
);

// Creating the associated component
RUKEN_DECLARE_TAG_COMPONENT(TestTagComponent);

// Creating an exclusive component
RUKEN_DECLARE_EXCLUSIVE_COMPONENT(ExclusiveComponentTest, 
    RUKEN_DECLARE_FIELD(TestField, RkSize)
);