#pragma once

#include "Types/FundamentalTypes.hpp"
#include "JobSystem/Concurrency.hpp"
#include "JobSystem/JobSystem.hpp"

#include <atomic_queue/atomic_queue.h>
#include <condition_variable>
#include <coroutine>

BEGIN_RUKEN_NAMESPACE

struct JobSystem;
struct SingleThreadSingleQueueExecutor;

#pragma warning(push)
#pragma warning(disable: 4324)
// Disabled warning 4324: Structure was padded due to __declspec(align())
// This is related to the way atomic values interacts with cache lines and is expected of the atomic_queue

/**
 * @brief A primitive allowing the categorization and prioritization of tasks.
 *
 * Due to the asynchronous nature of the job system, workers
 * don't just execute tasks right away, and use instead a level of indirection
 * called a job queue. These queues allow the job system to categorize and
 * prioritize some tasks over others, to minimize latency as well as optimize throughout
 * as much as possible.
 */
class JobQueue
{
	friend JobSystem;						 //
	friend SingleThreadSingleQueueExecutor; // calls OnRegister()

	#pragma region Members

	// Trace
	std::string m_name {};

	// Job queue
    atomic_queue::AtomicQueueB2<std::coroutine_handle<>> m_queue;

	// Worker synchronisation
	BinaryTreePath       m_request_location {};
	WorkerRequestTree*   m_request_tree     {nullptr};
	std::atomic<RkSize>  m_concurrency      {Concurrency().packed_value};

	static inline thread_local JobQueue* th_last_push_queue		   {nullptr};
	static inline thread_local RkBool    th_last_push_emit_request {false};

	#pragma endregion

    #pragma region Methods

	/**
	 * Called by JobSystem on registration.
	 * @param in_request_tree Request tree.
	 * @param in_request_location Request location.
	 */
	RkVoid OnRegister(WorkerRequestTree& in_request_tree, BinaryTreePath const& in_request_location) noexcept;

	/**
	 * Consumes a worker request if the concurrency allows it.
	 * @param inout_concurrency Current concurrency of the queue.
	 */
	RkBool TryConsumeWorkerRequest(Concurrency& inout_concurrency) noexcept;

	/**
	 * Emits a worker request if the concurrency allows it.
	 * @param inout_concurrency Current concurrency of the queue.
	 */
	RkBool TryEmitWorkerRequest(Concurrency& inout_concurrency) noexcept;

	/**
	 * @brief A simple utility function that attempts to dequeue a job and run it.
	 * @param in_max_attempts Maximum amount of times the operation can be attempted before returning.
	 *        A value of 0 will do nothing.
	 */
	RkVoid TryConsumeJob(RkUint32 in_max_attempts) noexcept;

    #pragma endregion

    public:
        
        #pragma region Lifetime

        /**
      	 * @brief Default constructor
         * @param in_size Size of the queue
         */
        explicit JobQueue(RkSize in_size) noexcept;
		JobQueue& operator=(JobQueue const&) = delete;
		JobQueue& operator=(JobQueue&&)      = delete;
        JobQueue           (JobQueue const&) = delete;
        JobQueue           (JobQueue&&)      = delete;
        ~JobQueue() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * @brief Blocking push, waits for available space in the queue
         * @param in_handle Job handle to push
         */
        RkVoid Push(std::coroutine_handle<> in_handle) noexcept;

        /**
         * @brief Attempts to consume jobs of the queue until the queue no longer requires this much concurrency.
         * @param in_stop_token Stop token.
         */
        RkVoid RunMultiple(std::stop_token const& in_stop_token);
		RkVoid RunOnce	  ();

		/// @brief Sets the maximum concurrency of the queue
		RkVoid SetMaximumConcurrency(RkUint16 in_max_concurrency) noexcept;

        #pragma endregion
};

#pragma warning(pop)

END_RUKEN_NAMESPACE