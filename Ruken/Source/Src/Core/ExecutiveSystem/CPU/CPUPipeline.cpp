#include "Core/ExecutiveSystem/CPU/CPUPipeline.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

CPUPipeline::CPUPipeline(const std::string_view in_name, std::vector<QueueInfo>&& in_queue_infos) noexcept:
	name        {in_name},
	queue_infos {in_queue_infos}
{
	// Priority is given to processing queues with at least one worker reserved for transfer


}