#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"
#include "Core/ExecutiveSystem/GPU/GraphicsProcessingQueue.hpp"
#include "Core/ExecutiveSystem/CPU/Events/CountDownLatch.hpp"
#include "Core/ExecutiveSystem/CPU/Events/ManualResetEvent.hpp"

USING_RUKEN_NAMESPACE

struct EcsQueue final: CentralProcessingQueue<EcsQueue, 4082>
{};

struct GraphicsQueue final: GraphicsProcessingQueue<GraphicsQueue>
{};

Task<EcsQueue> HelloTask(CountDownLatch& in_latch, RkSize const in_value)
{
    std::cout << "Hello from " + std::to_string(in_value) + ' ' + WorkerInfo::name + '\n';

    in_latch.CountDown();

    co_return;
}

Task<EcsQueue> SomeTask()
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
    cpu.SetConfiguration({
        EcsQueue::GetInterface(), EcsQueue::GetInterface(), EcsQueue::GetInterface()
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    SomeTask();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}