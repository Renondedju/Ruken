#include <iostream>
#include <vector>
#include <ranges>

#include "Utility/Benchmark.hpp" 
#include "Core/ExecutiveSystem/Task.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CPUQueueHandle.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CPUDynamicQueue.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Algorythms/WhenAll.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/CPUDynamicTask.hpp"

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue final: CPUQueueHandle<MainQueue, 4096> {};

/**
 * \brief Logs: "[<WorkerName>] Running job <in_identifier> - Concurrency [<Current>/<Optimal>]\n"
 * \param in_identifier Identifier of the job
 * \return Handle to the task instance
 */
CPUDynamicTask<> Job(RkSize const in_identifier)
{
    // ConcurrencyCounter const counter = MainQueue::instance.GetConcurrencyCounter();
    //
    //std::cout << std::format("[{}] Running job {} - Concurrency [{}/{}]\n", 
    //    WorkerInfo::name, in_identifier, counter.current_concurrency, counter.optimal_concurrency);
    
    co_return;
}

CPUDynamicTask<> WhenAll(std::vector<CPUDynamicTask<>> const& in_jobs)
{
    CountDownLatch               latch         {in_jobs.size()};
    std::vector<CPUContinuation> continuations {in_jobs.size()};

    for (RkSize index = 0; index < in_jobs.size(); ++index)
    {
        // Creating a job and keeping a reference onto it until the attachment
        // process has been done to avoid it being deleted in the meanwhile.

        // Instantiating the job and setup the continuation callback
        continuations[index].Setup(latch, in_jobs[index]);

        // Trying to attach the continuation and if the process failed, that means the event we
        // were looking to await has already been completed. Thus we need to decrement manually the latch by one.
        if (!continuations[index].TryAttach())
            latch.CountDown();
    }

    co_await latch;
}

/**
 * \brief Starts and waits asynchronously for in_size dummy jobs
 * \param in_exit_signal Signal reference to request the main thread termination of the program
 * \param in_size Amount of dummy jobs to start concurrently
 * \return Handle to the task instance
 */
Task<MainQueue> RunJobs(std::stop_source& in_exit_signal, RkSize const in_size)
{
    RUKEN_BENCHMARK("Run & await")
    {
        std::vector<CPUDynamicTask<>> jobs {in_size};
        for (auto [index, job]: std::views::enumerate(jobs))
            job = Job(index);

        co_await WhenAll(jobs);
    }

    std::cout << std::format("[{}] Done ! Requesting exit.\n", WorkerInfo::name);

    in_exit_signal.request_stop();
}

int main()
{
    ServiceProvider service_provider {};
    std::string     failure_reason   {};

    Scheduler*   scheduler    {service_provider.ProvideService<Scheduler  >(failure_reason)};
    EntityAdmin* entity_admin {service_provider.ProvideService<EntityAdmin>(failure_reason)};

    entity_admin->CreateSystem<CounterSystem>();

    for (int index = 0; index < 10000000; ++index)
        entity_admin->CreateEntity<CounterComponent>();

    entity_admin->ExecuteEvent(EEventName::OnStart);
    entity_admin->ExecuteEvent(EEventName::OnUpdate);

    /*
    Kernel kernel;

    return kernel.Run();
    */
}
