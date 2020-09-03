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

#pragma once

#include <atomic>
#include <vector>
#include <functional>

#include "Config.hpp"

#include "Core/Service.hpp"
#include "Debug/Logging/Logger.hpp"

#include "Types/Unique.hpp"
#include "Types/FundamentalTypes.hpp"

#include "Threading/Worker.hpp"
#include "Threading/ThreadSafeLockQueue.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class is responsible for the repartition of different tasks between workers
 */
class Scheduler final: public Service<Scheduler>, Unique
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

        #pragma region Constructors

        /**
         * \brief Scheduler constructor
         * \param in_service_provider Service provider
         * \param in_workers_count Number of managed workers
         */
        Scheduler(ServiceProvider& in_service_provider, RkUint16 in_workers_count = 0u);

        Scheduler(Scheduler const& in_copy)     = delete;
        Scheduler(Scheduler&& in_move) noexcept = delete;
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

        Scheduler& operator=(Scheduler const& in_copy)        = delete;
        Scheduler& operator=(Scheduler&& in_move) noexcept    = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE