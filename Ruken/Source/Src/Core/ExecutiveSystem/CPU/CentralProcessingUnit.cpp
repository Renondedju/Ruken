#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"

USING_RUKEN_NAMESPACE

RkVoid CentralProcessingUnit::SetConfiguration(std::vector<ICentralProcessingQueue> const& in_queues) noexcept
{
	if (m_workers.size() < in_queues.size())
	{
	    m_workers = std::vector<Worker>(in_queues.size());

		for (RkSize index {0ULL}; index < m_workers.size(); index++)
			new (&m_workers[index]) Worker(std::string("Worker ") + std::to_string(index));
	}

	// Set the new configurations
	for (RkSize index {0ULL}; index < in_queues.size(); index++)
	    m_workers[index].SetQueue(in_queues[index]);
}