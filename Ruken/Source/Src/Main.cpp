#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"
#include "Core/ExecutiveSystem/GPU/GraphicsProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

struct EcsQueue final: CentralProcessingQueue<EcsQueue, 4082>
{};

struct GraphicsQueue final: GraphicsProcessingQueue<GraphicsQueue>
{};

Task<GraphicsQueue> HelloTask()
{
    co_return;
}

Task<EcsQueue> SomeTask()
{
    std::cout << "Hello Task" << std::endl;

    co_return;
}

int main()
{
    Kernel kernel;

    return kernel.Run();
}