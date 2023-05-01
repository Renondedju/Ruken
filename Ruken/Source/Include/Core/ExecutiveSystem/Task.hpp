#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUTask.hpp"

BEGIN_RUKEN_NAMESPACE

template <QueueHandleType TQueueHandle, typename TReturnType = RkVoid>
using Task = std::conditional_t<std::is_same_v<typename TQueueHandle::ProcessingUnit, CentralProcessingUnit>,
    CPUTask<TQueueHandle, TReturnType>,
    RkVoid
    >;

END_RUKEN_NAMESPACE