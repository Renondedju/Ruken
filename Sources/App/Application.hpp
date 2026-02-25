#pragma once

#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/AsyncTask.hpp"

USING_RUKEN_NAMESPACE

struct MainQueue : QueueHandle<MainQueue, 64>{};

/// @brief Main application structure.
struct Application
{
	ServiceProvider root_services {"Application"};

	#pragma region Methods

	/**
	 * Runs async main and logs any caught exceptions.
	 *
	 * @param in_stop_source Program stop source.
	 */
	AsyncTask<MainQueue> TryCatchAsyncMain(std::stop_source& in_stop_source);

	/**
	 * @brief Main application asynchronous task.
	 *
	 *      This task is always ran on the main thread because of window management.
	 * When run, basic systems such as debug handlers/callbacks, clock, logging
	 * and jobs have already been provided/installed. Initialisation from now on can be multithreaded.
	 *
	 * Execution flowing off this task will destroy every root services and exit the
	 * process without waiting for any remaining task. AsyncMain is responsible
	 * for waiting for any critical piece of code to end before returning.
	 */
	AsyncTask<MainQueue> AsyncMain();

	/// @brief A test main I use to debug stuff.
	AsyncTask<MainQueue> AsyncTestMain();

	#pragma endregion
};