#include <iostream>

#include "Utility/Benchmark.hpp" 
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Algorythms/WhenAll.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue final: CPUQueueHandle<MainQueue, 4096> {};

// Logs: "[<WorkerName>] Running job <in_identifier> - Concurrency [<Current>/<Max>]\n"
Task<MainQueue, RkSize> Job(RkSize const in_identifier)
{
    ConcurrencyCounter const counter = MainQueue::queue.GetConcurrencyCounter();

    std::cout << "[" + WorkerInfo::name + "] Running job " + std::to_string(in_identifier) + 
                 + " - Concurrency [" + std::to_string(counter.current_concurrency) + "/"
                                      + std::to_string(counter.optimal_concurrency) + "]\n";

    co_return in_identifier;
}

// Starts and waits for in_amount of dummy tasks before requesting in_exit_signal
Task<MainQueue> RunJobs(std::stop_source& in_exit_signal, RkSize in_size)
{
    //Task<MainQueue, RkSize> task = Job(90);

    std::cout << co_await Job(50);
    //std::cout << co_await task;

    std::cout << '[' << WorkerInfo::name << "] Done ! Requesting exit.\n";
    in_exit_signal.request_stop();

    co_return;
}

int main()
{
    // CPU will slowly start threads and create workers if the machine has at least 2 logic cores.
    // Any async call can be ran from any other thread, synchronously or not without blocking the
    // current process
    CentralProcessingUnit cpu {};
    cpu.RegisterQueue(MainQueue::queue);

    RUKEN_BENCHMARK("Main")
    {
        // However, if the main thread exits right away, the cpu will be destroyed, resulting
        // in halting any ongoing task and killing all the workers.
        // To avoid this issue, a stop source is passed to the async function that will signal it upon completion.
        std::stop_source stop_signal;

        RunJobs(stop_signal, 10000); // < asynchronous call, does not block the thread

        // Finally we can actually wait for the signal by making this thread available to the CPU
        cpu.CallerAsWorker(stop_signal.get_token());
    }
}