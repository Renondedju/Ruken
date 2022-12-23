#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

CentralProcessingUnit::CentralProcessingUnit() noexcept
{
    m_workers.reserve(std::thread::hardware_concurrency() - 1);

	for (RkSize index = 0ULL; index < std::thread::hardware_concurrency() - 1; ++index)
        m_workers.emplace_back(std::make_unique<Worker>("CPU " + std::to_string(index), m_queues));

	WorkerInfo::name = std::string("CPU Main");
}

RkVoid CentralProcessingUnit::RegisterQueue(CentralProcessingQueue& in_queue) noexcept
{
    m_queues.emplace_back(std::addressof(in_queue));
}