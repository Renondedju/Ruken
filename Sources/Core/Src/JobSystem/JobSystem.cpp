#include "JobSystem/WorkerInfo.hpp"
#include "JobSystem/JobSystem.hpp"

#include "Debug/Logging/Logger.hpp"
#include "JobSystem/Queues/JobQueue.hpp"

USING_RUKEN_NAMESPACE

JobSystem::JobSystem(ServiceProvider& in_provider) noexcept:
    Service(in_provider)
{}

RkVoid JobSystem::RegisterQueue(JobQueue& in_queue) noexcept
{
    m_queues.emplace_back(std::addressof(in_queue));
}

RkVoid JobSystem::StartWorkers() noexcept
{
    RkSize const concurrency {std::thread::hardware_concurrency() - 1};

    m_service_provider.LocateService<Logger>()->Info(service_name, "Starting job system with {} worker(s) and {} queue(s)", concurrency, m_queues.size());

    m_workers.reserve(concurrency);

    for (RkSize index = 0ULL; index < concurrency; ++index)
        m_workers.emplace_back(std::make_unique<Worker>("CPU " + std::to_string(index), m_queues));

    WorkerInfo::name = std::string("CPU Main");
    tracy::SetThreadNameWithHint(WorkerInfo::name.c_str(), 1);
}

RkVoid JobSystem::CallerAsWorker(std::stop_token&& in_should_return) const noexcept
{
    while (!in_should_return.stop_requested())
        Worker::ProcessQueues(m_queues, in_should_return);
}
