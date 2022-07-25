#include <functional>

#include "Core/ExecutiveSystem/CPU/Worker.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"

USING_RUKEN_NAMESPACE

Worker::Worker(std::string&& in_name) noexcept:
    m_thread  {std::bind_front(&Worker::Routine, this), std::forward<std::string>(in_name)}
{}

RkVoid Worker::Routine(std::stop_token&& in_stop_token, std::string&& in_name) const noexcept
{
    WorkerInfo::name = in_name;

    // Waiting for a queue to be initially set
    while (!in_stop_token.stop_requested() && m_dequeue.load(std::memory_order_relaxed) == nullptr)
        ;

    std::coroutine_handle<> handle;

    // This loop needs to be as small as possible in order to reduce latency
    while (!in_stop_token.stop_requested())
        if (m_dequeue.load(std::memory_order_relaxed)(handle, 5000))
            handle.resume();
}

RkVoid Worker::SetQueue(ICentralProcessingQueue const& in_queue)
{
    m_dequeue.store(in_queue.try_dequeue, std::memory_order_release);
}