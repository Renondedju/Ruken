#pragma once


#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Central processing unit manual reset event
 */
using ManualResetEvent = CPUAwaitable;

END_RUKEN_NAMESPACE