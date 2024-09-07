#include "ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUQueue.hpp"

USING_RUKEN_NAMESPACE

RkVoid CentralProcessingUnit::RegisterQueue(CPUQueue& in_queue) noexcept
{
    m_queues.emplace_back(std::addressof(in_queue));
}

RkVoid CentralProcessingUnit::StartWorkers() noexcept
{
    RkSize const concurrency {std::thread::hardware_concurrency() - 7};

    m_workers.reserve(concurrency);

    for (RkSize index = 0ULL; index < concurrency; ++index)
        m_workers.emplace_back(std::make_unique<Worker>("CPU " + std::to_string(index), m_queues));

    WorkerInfo::name = std::string("CPU Main");
}

RkVoid CentralProcessingUnit::CallerAsWorker(std::stop_token&& in_should_return) const noexcept
{
    while (!in_should_return.stop_requested())
        Worker::ProcessQueues(m_queues, in_should_return);
}
