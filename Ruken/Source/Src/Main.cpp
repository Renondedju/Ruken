#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/CPUPipeline.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Events/CountDownLatch.hpp"
#include "Core/ExecutiveSystem/CPU/Events/ManualResetEvent.hpp"

USING_RUKEN_NAMESPACE

// --- Queues

// If a high priority queue has dependencies on a low priority one:
// Add the dependent weight of the parent on the awaited queue
// (2 threads working out of 5 optimal possible then concurrency of the awaited queue ups by 3) (or not we could just up the policy)
// This allows to take into account the blocking dependencies only, otherwise
// low priority queue will be treated on their own when time is found

// ECPUPipelinePolicy::TimeConstrained with a critical latency value in ms ?
// What about the pathfinding, a critical latency - could - make sense..
// How to detect stalls ? "stop or cause to stop making progress."
// A simulation could tell us: How fast should we do it, and who are we waiting for (aka. blocking queues -> dynamic) ?:
//    We can compute an estimation of the concurrency needed: if target is done in 10ms with 6 workers and the required is 16.6ms max,
//    then 10ms async * 6 ~= 60ms sync. 60ms sync / 16.6ms sync = 3.6144 workers required;
//    To prevent eventual lag spikes we reserve at least 30% of processing power available, so 3.6144 + 3.6144 * 0.3 (marge) = 4.69 workers
// This strategy only applies to time constrained queues on the long term, and doesn't divide the actual work.
// If the strategy is to do everything asap: the system will naturally max out quickly.

// +1 optimal concurrency when task is queued, -1 when paused
// Delayable should be called "take the rest of the frame"
// This carries optimal thread count from queue to queue automatically
// If dependency is to another processing unit (need to rename to device) then -1 concurrency
// (how to pull via a bridge optimally ?)

// Bridge is manipulated by the requesting device
// One bridge per subscription but pulling is scheduled by the device (aka done when needed)
// Special containers in the queues ?
// If a queue has nothing else to be done yet -> pull dependencies for those devices
// Pulling should depend on the strategy of the queue:
// - Minimize latency -> pull when nothing else to do: uses up a thread
// - Maximize fps     -> pulls lazily or at recurring intervals: no thread usage, but potential added latency (overlaps frames between devices)
//                       ^ requires a frequency rather than a time span / What for ?

// Max FPS             | [TimeConstrained] - 0.1ms required  
// Target 60           | [TimeConstrained] - 16.6ms required 
// Pathfinding         | [Interruptable]   - Take the rest (will be prioritized if required by time constrained queues)
// Editor Light-mapper | [Interruptable]   - Mainly editor queues

// If a TimeConstrained queue has a dependency on an Interruptable one, then the Interruptable is given the priority

// for (Queue& queue: queues_sorted_by_priority)
// {
//      if (queue.RequiresMoreWorkers())
//          while(queue.KeepWorker()) <- as long as there isn't a worker that is useless
//				queue.PopAndRun();
// } 

// Workers are autonomous
// If the worker of a previous pipeline is still working on an interruptable queue
// and isn't required by the new one, it will continue its work in the background
// In the case where all threads are required by the current pipeline the worker will naturally be given an order to
// switch to a more requested queue

// Calls from TimeConstrained queues to an Interruptable one will reserve new threads 

// Now how do I give out resources to the rest of the interruptable queues ?

struct ResourceManagement final: CPUQueueHandle<ResourceManagement, 4082> {};
struct PathfindingQueue   final: CPUQueueHandle<PathfindingQueue  , 4082> {};
struct SoundQueue         final: CPUQueueHandle<SoundQueue        , 4082> {};
struct EcsQueue           final: CPUQueueHandle<EcsQueue          , 4082> {};

// --- Tasks

Task<EcsQueue> HelloTask(CountDownLatch& in_latch, RkSize const in_value)
{
    std::cout << "Hello from " + std::to_string(in_value) + ' ' + WorkerInfo::name + '\n';

    in_latch.CountDown();

    co_return;
}

Task<EcsQueue> Main()
{
	constexpr RkSize count {2048ULL};
	CountDownLatch latch(count);

    for (RkSize i = 0ULL; i < count; ++i)
		HelloTask(latch, i);

    co_await latch;

    std::cout << "Done !\n";

    co_return;
}

int main()
{
    const CPUPipeline pipeline("Kernel Initialization Pipeline", {
        { EcsQueue  ::queue, 0 },
		{ SoundQueue::queue, 0 },
		{ IOQueue   ::queue, 2 }
    });

    CentralProcessingUnit cpu(pipeline);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    Main();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}