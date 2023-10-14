#include <iostream>
#include <vector>

#include "Utility/Benchmark.hpp" 
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Algorythms/WhenAll.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue final: CPUQueueHandle<MainQueue, 4096> {};

/**
 * \brief Logs: "[<WorkerName>] Running job <in_identifier> - Concurrency [<Current>/<Optimal>]\n"
 * \param in_identifier Identifier of the job
 * \return Handle to the task instance
 */
Task<MainQueue> Job(RkSize const in_identifier)
{
    // ConcurrencyCounter const counter = MainQueue::instance.GetConcurrencyCounter();
    //
    //std::cout << std::format("[{}] Running job {} - Concurrency [{}/{}]\n", 
    //    WorkerInfo::name, in_identifier, counter.current_concurrency, counter.optimal_concurrency);
    
    co_return;
}

/**
 * \brief Starts and waits asynchronously for in_size dummy jobs
 * \param in_exit_signal Signal reference to request the main thread termination of the program
 * \param in_size Amount of dummy jobs to start concurrently
 * \return Handle to the task instance
 */
Task<MainQueue> RunJobs(std::stop_source& in_exit_signal, RkSize const in_size)
{
    RUKEN_LOOPED_BENCHMARK("Run & Await", 50)
    {
        CountDownLatch               latch         {in_size};
        std::vector<CPUContinuation> continuations {in_size};

        for (RkSize index = 0; index < in_size; ++index)
        {
            // Creating a job and keeping a reference onto it until the attachment
            // process has been done to avoid it being deleted in the meanwhile.
            auto job = Job(index);

            // Instantiating the job and setup the continuation callback
            continuations[index].Setup(latch, job);

            // Trying to attach the continuation and if the process failed, that means the event we
            // were looking to await has already been completed. Thus we need to decrement manually the latch by one.
            if (!continuations[index].TryAttach())
                latch.CountDown();
        }

        co_await latch;

        //std::cout << std::format("Ran batch {}/{}\n", benchmark_iterator.loop_index + 1, benchmark_iterator.loop_count);
    }

    std::cout << std::format("[{}] Done ! Requesting exit.\n", WorkerInfo::name);

    in_exit_signal.request_stop();
}

int main()
{
    // CPU will slowly start threads and create workers if the machine has at least 2 logic cores.
    // Any async call can be ran from any other thread, synchronously or not without blocking the
    // current process
    CentralProcessingUnit cpu {};
    cpu.RegisterQueue(MainQueue::instance);

    RUKEN_BENCHMARK("Main")
    {
        // However, if the main thread exits right away, the cpu will be destroyed, resulting
        // in halting any ongoing task and killing all the workers.
        // To avoid this issue, a stop source is passed to the async function that will signal it upon completion.
        std::stop_source stop_signal;

        RunJobs(stop_signal, 100000); // < asynchronous call, does not block the thread

        // Finally we can actually wait for the signal by making this thread
        // available to the CPU until a stop is requested
        cpu.CallerAsWorker(stop_signal.get_token());
    }
}