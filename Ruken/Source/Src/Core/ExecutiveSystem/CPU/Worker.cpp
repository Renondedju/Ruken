#include <functional>

#include "Core/ExecutiveSystem/CPU/Worker.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/Queues/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

Worker::Worker(std::string in_name, std::vector<CentralProcessingQueue*>& in_queues) noexcept:
    m_queues {in_queues},
    m_thread {std::bind_front(&Worker::Routine, this), std::move(in_name)}
{}

RkVoid Worker::ProcessQueues(std::vector<CentralProcessingQueue*> const& in_queues, std::stop_token const& in_stop_token) noexcept
{
    // This obviously defeats the whole purpose of making queues but is only temporary
    // and is meant to be customized for each projects based on the needs.
    // TODO: Implement a way of customizing the process behavior without modifying this code
    for (CentralProcessingQueue* queue: in_queues)
    {
        WorkerInfo::current_queue = queue;
        queue->PopAndRun(true, in_stop_token);
    }
}

RkVoid Worker::Routine(std::stop_token&& in_stop_token, std::string&& in_name) const noexcept
{
    WorkerInfo::name = in_name;

    // This loop needs to be as small as possible in order to reduce latency
    while (!in_stop_token.stop_requested())
        Worker::ProcessQueues(m_queues, in_stop_token);
}
