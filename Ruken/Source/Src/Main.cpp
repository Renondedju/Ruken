#include <iostream>

#include "Core/Kernel.hpp"
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Events/CountDownLatch.hpp"
#include "Core/ExecutiveSystem/CPU/Events/ManualResetEvent.hpp"

// Debug
#include "Utility/Benchmark.hpp" 

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
// Pathfinding         | [Continuous]      - Must be ran by at least one worker (will be prioritized if required by time constrained queues)
// Editor Light-mapper | [Interruptable]   - Mainly editor queues

// If a TimeConstrained queue has a dependency on an Interruptable one, then the Interruptable is given the priority

// for (Queue& queue: queues_sorted_by_priority)
// {
//      if (queue.RequiresMoreWorkers()) // (target occupancy not met for TimeConstrained queues)
//          while(queue.PopAndRun());
// } 

// Big loop in workers as long as a queue decides to "stick" to it
// OR 
// Global state machine on workers << seems easier to reason about + reduces the cost of the checks gradually from worsts to cheaper ones

// std::atomic<Worker*> m_continuous_worker; - Is the worker that will always prioritize continuous queues over anything else
// Other workers will adjust based on the requirements 

// t(Q) = target latency of a queue Q              - 16 ms
// l(Q) = current latency of Q                     - 10 ms
// a(Q) = total amount of time worked              - 10 ms
// c(Q) = current concurrency of the queue         - 2
// m(Q) = maximum current concurrency of the queue - 8
// W    = number of available workers              - 2

// balance = l(Q) / t(Q)
// - a value > 1 means that too much resources are given to the queue 
// - a value < 1 means that more resources need to be allocated to reach the target in time

// Required concurrency: R = W * balance
// Relative priority       = min(R - c(Q), m(Q))

// ------ CONTINUOUS QUEUES ------
// 
// Relative priority should always be one such as:
//     min(R - c(Q), m(Q))        = 1
// <=> min(W * balance, m(Q))     = 1
// <=> min(W * l(Q) / t(Q), m(Q)) = 1
//
// What that basically means is that either or both of "m(Q)"
// and "W * l(Q) / t(Q)" has to = 1:
//
//     W * l(Q) / t(Q)      = 1     => Here we want W to be controlled by other systems so we will consider it a constant
//                                     that means that we need to compensate the value of W with l(Q) / t(Q)
// <=> W * (l(Q)*1)/(t(Q)*W) = 1    => By multiplying the other term by a factor of 1/W we can make sure we don't modify l(Q)
//                                     because this is not the main goal, a continuous queue just wants to run continuously,
//                                     no matter the load of the system.
// <=> l(Q)/(t(Q)*W) = 1/W
// <=> l(Q)          = (t(Q)*W)/W
// <=> l(Q)          = t(Q)


// ________________________
//
// This way m(Q) can also freely determine if the queue should
// be ran in the case m(Q) > 1 meaning that there is work to be done.
//
// ------ INTERRUPTABLE QUEUES ------
// We can use the same principle as the continuous queue, although we basically want to
// share the remaining resources among them, meaning that every interruptable queue should
// have the same priority of 1 when spare resources are available
// 
// By putting t(Q) = l(Q)*W/len(Q), we can make sure that the load will be evenly distributed
// Hopefully that should allow us to get rid of the branches and be vectorized by the compiler
// We need evaluations to be pretty quick to allow to be called more frequently and increase the precision
// even though this has to be profiled

// We can subdivide W into 3 distinct values
// W = Wt + Wc + Wi
// 

// Efficiently sorting and updating queues

struct ResourceManagement final: CPUQueueHandle<ResourceManagement, 4082> {};
struct PathfindingQueue   final: CPUQueueHandle<PathfindingQueue  , 4082> {};
struct SoundQueue         final: CPUQueueHandle<SoundQueue        , 4082> {};
struct EcsQueue           final: CPUQueueHandle<EcsQueue          , 4082> {};

// --- Tasks

Task<EcsQueue> HelloTask(CountDownLatch& in_latch, RkSize const in_value)
{
    ConcurrencyCounter const counter = EcsQueue::queue.GetConcurrencyCounter();

    std::cout << "- Running job " + std::to_string(in_value) + " [" + WorkerInfo::name
                + "] : " + std::to_string(counter.current_concurrency) + " current workers out of optimal maximum: "
                         + std::to_string(counter.optimal_concurrency) + '\n';
    
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
    CentralProcessingUnit cpu {};
    cpu.RegisterQueue(EcsQueue::queue);
    cpu.RegisterQueue(SoundQueue::queue);
    cpu.RegisterQueue(PathfindingQueue::queue);
    cpu.RegisterQueue(ResourceManagement::queue);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    Main();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}