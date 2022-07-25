
#include "Build/Config.hpp"
#include "Threading/Scheduler.hpp"
#include "Core/ServiceProvider.hpp"

USING_RUKEN_NAMESPACE

Scheduler::Scheduler(ServiceProvider& in_service_provider, RkUint16 const in_workers_count) noexcept:
    Service<Scheduler> {in_service_provider},
    m_workers          {in_workers_count == 0U ? std::thread::hardware_concurrency() - 1 : in_workers_count},
    m_running          {true}
{
    #if defined(RUKEN_LOGGING_ENABLED)

        m_logger = m_service_provider.LocateService<Logger>()->AddChild("scheduler");
        m_logger->Info("Spawning " + std::to_string(m_workers.size()) + " workers");

    #endif

    for (OldWorker& worker : m_workers)
        worker.Execute(&Scheduler::WorkersJob, this);
}

Scheduler::~Scheduler()
{
    Shutdown();
}

RkVoid Scheduler::ScheduleTask(Job&& in_task) noexcept
{
    if (!m_running.load(std::memory_order_acquire))
        return;

    m_job_queue.Enqueue(std::forward<Job>(in_task));
}

RkVoid Scheduler::WaitForQueuedTasks() noexcept
{
    if (!m_running.load(std::memory_order_acquire))
        return;

    while (!m_job_queue.Empty())
        std::this_thread::yield();
}

RkVoid Scheduler::Shutdown() noexcept
{
    if (!m_running.load(std::memory_order_acquire))
        return;

    m_running.store(false, std::memory_order_release);
    m_job_queue.Clear();

    for (OldWorker& worker : m_workers)
        worker.Detach();
}

std::vector<OldWorker> const& Scheduler::GetWorkers() const noexcept
{
    return m_workers;
}

RkVoid Scheduler::WorkersJob() noexcept
{
    Job current_job;

    while (m_running.load(std::memory_order_acquire))
    {
        // Always try to dequeue jobs, the job queue will lock us if nothing is available
        RkBool const job_validity = m_job_queue.Dequeue(current_job);

        if (job_validity)
            current_job();
    }
}
