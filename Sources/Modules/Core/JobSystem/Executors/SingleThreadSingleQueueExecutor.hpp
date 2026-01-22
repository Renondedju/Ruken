#pragma once

#include "JobSystem/Queues/JobQueue.hpp"

BEGIN_RUKEN_NAMESPACE

/// TODO: This class is a workaround. JobSystem has been designed for maximum concurrency and cannot target a
///		  specific thread easily without impacting performance in a noticeable way.
///		  This is mostly meant for the main thread, because some libraries (GLFW, OpenGL...) cannot be called on
///		  another thread, even with proper synchronisation.
///
/// This way of doing things is not super elegant and I would like
/// to redesign things, maybe with a proper executor interface ? What about the integration in services ?
struct SingleThreadSingleQueueExecutor
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_queue Queue to work on.
	 */
	explicit SingleThreadSingleQueueExecutor(JobQueue& in_queue) noexcept;
	SingleThreadSingleQueueExecutor			  (const SingleThreadSingleQueueExecutor&) = delete;
	SingleThreadSingleQueueExecutor			  (SingleThreadSingleQueueExecutor&&)      = delete;
	SingleThreadSingleQueueExecutor& operator=(const SingleThreadSingleQueueExecutor&) = delete;
	SingleThreadSingleQueueExecutor& operator=(SingleThreadSingleQueueExecutor&&)      = delete;
	~SingleThreadSingleQueueExecutor()												   = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * @brief Captures the calling thread and uses it as a worker.
	 * @param in_stop_token Stop token, the method won't return until a stop is requested.
	 * @param in_worker_name Name of the worker.
	 */
	RkVoid CallerAsWorker(std::stop_token&& in_stop_token, std::string in_worker_name) noexcept;

	#pragma endregion

	private:

		#pragma region Members

		WorkerRequestTree m_request_tree {1UZ};
		JobQueue&		  m_queue;

		#pragma endregion
};

END_RUKEN_NAMESPACE