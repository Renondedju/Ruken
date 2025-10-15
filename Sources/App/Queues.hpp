#pragma once

#include "Core/JobSystem/Queues/QueueHandle.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue       : QueueHandle<MainQueue      , 64  >{};
struct ProcessingQueue : QueueHandle<ProcessingQueue, 2048>{};