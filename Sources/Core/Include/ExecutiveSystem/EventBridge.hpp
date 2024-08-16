#pragma once

#include "ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingUnitType TEmittingUnit, ProcessingUnitType TReceivingUnit>
struct EventBridge
{};

END_RUKEN_NAMESPACE