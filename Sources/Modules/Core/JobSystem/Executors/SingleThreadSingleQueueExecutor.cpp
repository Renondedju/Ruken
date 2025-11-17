#include "core/JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp"

USING_RUKEN_NAMESPACE

SingleThreadSingleQueueExecutor::SingleThreadSingleQueueExecutor(JobQueue& in_queue) noexcept:
	m_queue {in_queue}
{
	in_queue.SetMaximumConcurrency(1ULL);
	in_queue.OnRegister			  (m_request_tree, BinaryTreePath {
		.path      = 0ULL,
		.depth     = m_request_tree.tree.max_depth,
		.max_depth = m_request_tree.tree.max_depth
	});
}

RkVoid SingleThreadSingleQueueExecutor::CallerAsWorker(
	std::stop_token&&      in_stop_token,
	std::string_view const in_worker_name) noexcept
{
	static std::atomic_flag s_threads {};

	RUKEN_ASSERT(s_threads.test_and_set() == false, "Only a single thread can be a worker at a time");

	JobSystem::worker_info.name			 = in_worker_name;
	JobSystem::worker_info.current_queue = &m_queue;
	tracy::SetThreadNameWithHint(JobSystem::worker_info.name.c_str(), 1);

	// -- Main Routine
	while (!in_stop_token.stop_requested())
	{
		m_request_tree.WaitConsumeRequest({}, in_stop_token);

		if (in_stop_token.stop_requested())
			continue;

		try {
			m_queue.RunMultiple(in_stop_token);
		}
		catch (std::exception& in_exception) {
			[[maybe_unused]] const char* what {in_exception.what()};
			TracyMessageC(what, strlen(what), 0xFF0000);
		}
	}

	s_threads.clear();
}
