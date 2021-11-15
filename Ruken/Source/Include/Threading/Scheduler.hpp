
#pragma once

#include <atomic>
#include <vector>
#include <functional>

#include "Core/Service.hpp"
#include "Build/Namespace.hpp"
#include "Threading/Worker.hpp"
#include "Debug/Logging/Logger.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Threading/ThreadSafeLockQueue.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class is responsible for the repartition of different tasks between workers
 */
class Scheduler final : public Service<Scheduler>
{
    public: using Job = std::function<RkVoid()>;

    private:

        #pragma region Members

        std::vector<Worker>      m_workers;
        std::atomic_bool         m_running;
        ThreadSafeLockQueue<Job> m_job_queue;

        Logger* m_logger;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Job given to every worker used my the scheduler
         */
        RkVoid WorkersJob() noexcept;

        #pragma endregion

    public:

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = RUKEN_STRING(Scheduler);

        #pragma endregion

        #pragma region Constructors

        /**
         * \brief Scheduler constructor
         * \param in_service_provider Service provider
         * \param in_workers_count Number of managed workers
         */
        Scheduler(ServiceProvider& in_service_provider, RkUint16 in_workers_count = 0U) noexcept;

        Scheduler(Scheduler const& in_copy) = delete;
        Scheduler(Scheduler&&      in_move) = delete;
        ~Scheduler();

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Schedules a task on one of the available threads
         * \param in_task Task to schedule, any return value will be discarded
         * \note If Shutdown() has been called, this method has no effect
         */
        RkVoid ScheduleTask(Job&& in_task) noexcept;

        /**
         * \brief Waits until all the queued tasks are completed
         */
        RkVoid WaitForQueuedTasks() noexcept;

        /**
         * \brief Waits for all current active tasks to be done and drops any queued jobs. This also detaches any workers.
         * \note This method can only be called once
         */
        RkVoid Shutdown() noexcept;

        std::vector<Worker> const& GetWorkers() const noexcept;

        #pragma endregion 

        #pragma region Operators

        Scheduler& operator=(Scheduler const& in_copy) = delete;
        Scheduler& operator=(Scheduler&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE