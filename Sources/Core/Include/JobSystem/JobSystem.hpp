#pragma once

#include "Meta/Meta.hpp"

#include "Core/Service.hpp"
#include "Core/ServiceProvider.hpp"
#include "JobSystem/WorkerInfo.hpp"
#include "JobSystem/WorkerRequestTree.hpp"

#include <vector>
#include <thread>
#include <shared_mutex>

BEGIN_RUKEN_NAMESPACE

class ServiceProvider;
class JobQueue;

/**
 * This class contains the thread pool required to execute the different tasks.
 * Execution is immediate (greedy) and does not require submission.
 */
struct JobSystem final : Service
{
	using EvaluateWorkerBias = BinaryTreePath (*)(RkUint64 in_total, RkUint64 in_current, JobSystem& in_job_system);

	constexpr static           std::string_view service_name {RUKEN_STRING(JobSystem)};
	static inline thread_local WorkerInfo       worker_info  {};

    #pragma region Lifetime

    /**
     * Constructor.
     * @param in_provider Parent service provider.
     * @param in_queues Registered queues.
     * @param in_bias_function Worker bias evaluation function. Called every time the size of the worker pool changes.
     */
    explicit JobSystem(ServiceProvider&  in_provider,
	    std::initializer_list<JobQueue*> in_queues,
	    EvaluateWorkerBias			     in_bias_function) noexcept;

     JobSystem			 (JobSystem const&) = delete;
     JobSystem			 (JobSystem&&)      = delete;
	 JobSystem& operator=(JobSystem const&) = delete;
	 JobSystem& operator=(JobSystem&&)      = delete;
    ~JobSystem() override;

    #pragma endregion

    #pragma region Methods

    /**
     * @brief Captures the calling thread and uses it as a worker.
     * @param in_stop_token Stop token, the method won't return until a stop is requested.
     * @param in_worker_name Name of the worker.
     */
    RkVoid CallerAsWorker(std::stop_token&& in_stop_token, std::string_view in_worker_name) noexcept;

    /**
     * Returns the current amount of workers in the system.
     * This includes threads currently in the CallerAsWorker method.
     * @return Worker count.
     */
    RkSize GetWorkerCount() const noexcept;

    /**
     * Returns the path or bias of the passed queue in the worker request tree.
     * @param in_queue Queue to get the bias of.
     * @return Bias.
     */
    RkUint64 GetBiasForQueue(JobQueue const* in_queue) const noexcept;

    #pragma endregion

	private:

		#pragma region Members

		mutable std::shared_mutex                        m_workers_mtx {};
		std::unordered_map<std::thread::id, WorkerInfo*> m_workers_map {};
		std::vector		  <JobQueue*>					 m_queues	   {};
		std::vector       <std::jthread>				 m_workers     {};

		EvaluateWorkerBias m_bias_function {};
		WorkerRequestTree  m_request_tree;

		#pragma endregion

		#pragma region Methods

		/**
		 * @brief Runs jobs on the passed queues for a maximum of one full cycle.
		 * @param in_queue Queue to run jobs from.
		 * @param in_stop_token If a stop is requested, the method will return as soon as the current job is done
		 */
		static RkVoid ProcessQueue(JobQueue* in_queue, std::stop_token const& in_stop_token) noexcept;

		/**
		 * @brief (Re)evaluates a new queue bias for every worker.
		 */
		RkVoid EvaluateWorkerBiases() noexcept;

		#pragma endregion
};


END_RUKEN_NAMESPACE
