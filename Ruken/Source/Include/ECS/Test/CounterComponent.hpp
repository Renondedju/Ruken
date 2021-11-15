
#pragma once

#include <array>

#include "ECS/SparseComponent.hpp"
#include "ECS/TagComponent.hpp"
#include "ECS/ComponentField.hpp"
#include "ECS/ExclusiveComponent.hpp"

USING_RUKEN_NAMESPACE

// Creating fields
RUKEN_DEFINE_COMPONENT_FIELD(CountField		 , RkSize);
RUKEN_DEFINE_COMPONENT_FIELD(TestPaddingField, std::array<RkSize, 10>);

// Creating the associated component
RUKEN_DEFINE_COMPONENT	  (CounterComponent, CountField, TestPaddingField);
RUKEN_DEFINE_TAG_COMPONENT(TestTagComponent);

// --- 
RUKEN_DEFINE_COMPONENT_FIELD	(TestField, RkSize);
RUKEN_DEFINE_EXCLUSIVE_COMPONENT(ExclusiveComponentTest, TestField);