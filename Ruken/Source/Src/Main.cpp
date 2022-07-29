#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Events/CountDownLatch.hpp"
#include "Core/ExecutiveSystem/CPU/Events/ManualResetEvent.hpp"

USING_RUKEN_NAMESPACE

// --- Queues

struct PathfindingQueue final: CPUQueueHandle<PathfindingQueue, 4082, ECPUQueueGroup::Processing, ECPUPipelinePolicy::Delayable> {};
struct SoundQueue       final: CPUQueueHandle<SoundQueue      , 4082, ECPUQueueGroup::Processing, ECPUPipelinePolicy::Blocking > {};
struct EcsQueue         final: CPUQueueHandle<EcsQueue        , 4082, ECPUQueueGroup::Processing, ECPUPipelinePolicy::Blocking > {};
struct IOQueue          final: CPUQueueHandle<IOQueue         , 4082, ECPUQueueGroup::Transfer  , ECPUPipelinePolicy::Delayable> {};

// --- Tasks

Task<EcsQueue> HelloTask(CountDownLatch& in_latch, RkSize const in_value)
{
    std::cout << "Hello from " + std::to_string(in_value) + ' ' + WorkerInfo::name + '\n';

    in_latch.CountDown();

    co_return;
}

Task<EcsQueue> Main()
{
	constexpr RkSize count {512ULL};
	CountDownLatch latch(count);

    for (RkSize i = 0ULL; i < count; ++i)
		HelloTask(latch, i);

    co_await latch;

    std::cout << "Done !\n";

    co_return;
}

int main()
{
    CentralProcessingUnit cpu;
    cpu.SetConfiguration({ &EcsQueue::queue, &EcsQueue::queue, &EcsQueue::queue });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    Main();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}