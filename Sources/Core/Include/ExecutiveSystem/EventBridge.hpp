#pragma once

#include "ExecutiveSystem/Concepts/CProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

template <CProcessingUnit TEmittingUnit, CProcessingUnit TReceivingUnit>
struct EventBridge
{};

END_RUKEN_NAMESPACE