#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"
#include "Core/ExecutiveSystem/GPU/GraphicsProcessingQueue.hpp"
#include "Core/ExecutiveSystem/CPU/Events/ManualResetEvent.hpp"

USING_RUKEN_NAMESPACE

struct EcsQueue final: CentralProcessingQueue<EcsQueue, 4082>
{};

struct GraphicsQueue final: GraphicsProcessingQueue<GraphicsQueue>
{};

Task<EcsQueue> HelloTask()
{
    std::cout << "Hello from " << WorkerInfo::name << std::endl;

    co_return;
}

Task<EcsQueue> SomeTask()
{
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();
    HelloTask();

    std::cout << "Some Task" << std::endl;

    co_return;
}

int main()
{
    CentralProcessingUnit cpu;
    cpu.SetConfiguration({EcsQueue::GetInterface(), EcsQueue::GetInterface()});

    SomeTask();

    std::this_thread::sleep_for(std::chrono::seconds(2));
}