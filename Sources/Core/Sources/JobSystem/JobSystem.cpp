#include "JobSystem/Queues/JobQueue.hpp"
#include "JobSystem/WorkerInfo.hpp"
#include "JobSystem/JobSystem.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Meta/Assert.hpp"

#include <functional>
#include <ranges>

USING_RUKEN_NAMESPACE

RkVoid JobSystem::ProcessQueue(JobQueue* in_queue, RkBool const in_sticky, std::stop_token const& in_stop_token) noexcept
{
   worker_info.current_queue = in_queue;

    try {
        if (!in_sticky)
            in_queue->RunOnce();
        else
            in_queue->RunMultiple(in_stop_token);
    }
    catch (std::exception& in_exception) {
        const char* what {in_exception.what()};
        TracyMessageC(what, strlen(what), 0xFF0000);
    }
}

RkVoid JobSystem::EvaluateWorkerBiases() noexcept
{
    for (auto&& [index, pair] : std::views::enumerate(m_workers_map))
        pair.second->queue_bias = m_bias_function(m_workers_map.size(), index, *this);
}

JobSystem::JobSystem(
        ServiceProvider&                       in_provider,
        std::initializer_list<JobQueue*> const in_queues,
        EvaluateWorkerBias               const in_bias_function) noexcept:
    Service         {in_provider, typeid(JobSystem)},
    m_queues        {in_queues},
    m_bias_function {in_bias_function},
    m_request_tree  {in_queues.size()}
{
    RkSize const concurrency {std::thread::hardware_concurrency() - 1};

    m_service_provider.LocateService<Logger>()->Info(service_name,
        "Starting job system with {} worker(s) and {} queue(s) (excluding the main thread)", concurrency, m_queues.size());

    // Registering queues
    for (auto&& [index, queue]: std::views::enumerate(m_queues))
        queue->OnRegister(m_request_tree, BinaryTreePath {
            .path      = static_cast<RkUint64>(index),
            .depth     = m_request_tree.tree.max_depth,
            .max_depth = m_request_tree.tree.max_depth
        });

    // Starting workers
    m_workers.reserve(concurrency);
    for (RkSize index = 0ULL; index < concurrency; ++index)
        m_workers.emplace_back(
            std::bind_front(&JobSystem::CallerAsWorker, this),
            std::move      ("CPU " + std::to_string(index))
        );
}

JobSystem::~JobSystem()
{
    for (std::jthread& thread : m_workers)
        thread.request_stop();
}

RkVoid JobSystem::CallerAsWorker(std::stop_token&& in_stop_token, std::string_view const in_worker_name) noexcept
{
    { // Registering the thread as a worker
        std::lock_guard lock {m_workers_mtx};
        m_workers_map[std::this_thread::get_id()] = &worker_info;

        EvaluateWorkerBiases();
    }

    worker_info.name = in_worker_name;
    tracy::SetThreadNameWithHint(worker_info.name.c_str(), 1);

    // -- Main Routine
    while (!in_stop_token.stop_requested())
    {
        BinaryTreePath const path {m_request_tree.WaitConsumeRequest(worker_info.queue_bias, in_stop_token)};
        RkBool const is_parent_of {worker_info.queue_bias.IsParentOf(path)};

        if (!in_stop_token.stop_requested())
            ProcessQueue(m_queues[path.path], is_parent_of, in_stop_token);
    }

    { // Unregistering the thread
        std::lock_guard lock {m_workers_mtx};
        m_workers_map.erase(std::this_thread::get_id());
    }
}

RkSize JobSystem::GetWorkerCount() const noexcept
{
    std::lock_guard lock {m_workers_mtx};
    return m_workers_map.size();
}

RkUint64 JobSystem::GetBiasForQueue(JobQueue const* in_queue) const noexcept
{
    for (int i = 0; i < m_queues.size(); ++i)
        if (m_queues[i] == in_queue)
            return i;

    RUKEN_ASSERT(true,
        "in_queue must be passed to the JobSystem constructor "
        "before attempting to use it in JobSystem::GetBiasForQueue"
    );

    return 0;
}
