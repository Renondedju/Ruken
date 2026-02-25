// Engine headers
#include "Core/Time/Clock.hpp"
#include "Core/Debug/Logging/Logger.hpp"
#include "Core/Debug/Logging/Handlers/DebugHandler.hpp"
#include "Core/Debug/Logging/Handlers/ConsoleHandler.hpp"
#include "Core/JobSystem/JobSystem.hpp"
#include "Core/JobSystem/Queues/QueueHandle.hpp"
#include "Core/JobSystem/Executors/SingleThreadSingleQueueExecutor.hpp"
#include "ECS/ECSTask.hpp"
#include "Filesystem/IOJobQueue.hpp"

// App headers
#include "Application.hpp"

USING_RUKEN_NAMESPACE

/// @brief Initializes services and waits for the async main function to request a stop.
int main([[maybe_unused]] int   in_arg_count,
         [[maybe_unused]] char* in_arg_values[])
{
    // 1. --- Pre-initialization & Configuration ---
    std::set_terminate(&TerminateHandler);

    SingleThreadSingleQueueExecutor main_executor   {MainQueue::instance};
    ConsoleHandler                  console_handler {};
    DebugHandler                    debug_handler   {};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };
    std::initializer_list              queues   { &ECSJobQueue::instance, &IOJobQueue::instance };

    auto worker_bias_function = [](RkUint64 const, RkUint64 const in_current, JobSystem&) {
        return BinaryTreePath {
            .path  = in_current < 3ULL ? 0b1ULL : 0b0ULL,
            .depth = 1
        }; // The first 3 threads will prioritize the IO queue.
    };

    // 2. --- Initializing services and core systems ---
    Application application {};
    application.root_services.ProvideService<Clock>();
    application.root_services.ProvideService<Logger>(handlers);
    application.root_services.ProvideService<JobSystem>(queues, worker_bias_function);

    // 3. --- Finally, running async main ---
    std::stop_source  stop_source {};
    application.TryCatchAsyncMain(stop_source);

    // ... and waiting for it to complete as a worker.
    main_executor.CallerAsWorker(stop_source.get_token(), "CPU Main");

    return EXIT_SUCCESS;
}