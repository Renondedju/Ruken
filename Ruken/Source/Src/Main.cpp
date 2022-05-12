#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"
#include "Core/ExecutiveSystem/GPU/GraphicsProcessingQueue.hpp"

#include "Core/ExecutiveSystem/Subscriptions/DirectToDirectSubscription.hpp"

USING_RUKEN_NAMESPACE

struct EcsQueue final: CentralProcessingQueue<EcsQueue, 4082>
{};

struct GraphicsQueue final: GraphicsProcessingQueue<GraphicsQueue>
{};

Task<EcsQueue> HelloTask()
{
    std::cout << "Hello Task" << std::endl;

    co_return;
}

Task<EcsQueue> SomeTask()
{
    co_await HelloTask();

    std::cout << "Some Task" << std::endl;

    co_return;
}

int main()
{
    SomeTask();

    Kernel kernel;

    return kernel.Run();
}