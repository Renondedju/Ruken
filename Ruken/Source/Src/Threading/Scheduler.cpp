/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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

    for (Worker& worker : m_workers)
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

    for (Worker& worker : m_workers)
        worker.Detach();
}

std::vector<Worker> const& Scheduler::GetWorkers() const noexcept
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
