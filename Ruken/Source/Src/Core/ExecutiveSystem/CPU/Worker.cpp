#include <functional>

#include "Core/ExecutiveSystem/CPU/Worker.hpp"
#include "Core/ExecutiveSystem/CPU/WorkerInfo.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingQueue.hpp"

USING_RUKEN_NAMESPACE

Worker::Worker(std::string&& in_name) noexcept:
    m_thread {std::bind_front(&Worker::Routine, this), std::forward<std::string>(in_name)}
{}

RkVoid Worker::Routine(std::stop_token&& in_stop_token, std::string&& in_name) const noexcept
{
    WorkerInfo::name = in_name;

    // Waiting for a queue to be initially set
    while (!in_stop_token.stop_requested() && m_queue.load(std::memory_order_relaxed) == nullptr)
        ;

    std::coroutine_handle<> handle;

    // This loop needs to be as small as possible in order to reduce latency
    while (!in_stop_token.stop_requested())
        if (m_queue.load(std::memory_order_relaxed)->TryDequeue(handle, 5000))
            handle.resume();
}

RkVoid Worker::SetQueue(CentralProcessingQueue& in_queue)
{
    m_queue.store(std::addressof(in_queue), std::memory_order_release);
}