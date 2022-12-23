#include <functional>

#include "Core/ExecutiveSystem/CPU/Worker.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

Worker::Worker(std::string in_name, std::vector<CentralProcessingQueue*>& in_queues) noexcept:
    m_queues {in_queues},
    m_thread {std::bind_front(&Worker::Routine, this), std::move(in_name)}
{}

RkVoid Worker::Routine(std::stop_token&& in_stop_token, std::string&& in_name) const noexcept
{
    WorkerInfo::name = in_name;

    // This loop needs to be as small as possible in order to reduce latency
    while (!in_stop_token.stop_requested())
        for (CentralProcessingQueue* queue: m_queues)
            queue->PopAndRun(true, in_stop_token);
}