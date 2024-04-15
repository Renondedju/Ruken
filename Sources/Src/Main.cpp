#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/QueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : CPUQueueHandle<MainQueue, 2048>
{};

int main()
{
    CentralProcessingUnit cpu {};
    cpu.RegisterQueue(MainQueue::instance);

    Kernel kernel {};

    return kernel.Run();
}
