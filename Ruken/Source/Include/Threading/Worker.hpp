
#pragma once

#include <thread>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Worker
{
    private:

        #pragma region Variables

        std::thread m_thread {};

        #pragma endregion

    public:

        #pragma region Constructors

        Worker()                      = default;
        Worker(Worker const& in_copy) = delete;
        Worker(Worker&&      in_move) = default;
        ~Worker() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the worker is busy or not 
         * \return True if the worker is busy
         */
        [[nodiscard]]
        RkBool Available() const noexcept;

        /**
         * \brief Waits for the last task to execute, and starts the execution of this new job 
         * \tparam TExecutable Type of the job
         * \param in_job job to execute when done with the previous one
         * \param in_args Args of the job to execute
         */
        template <typename TExecutable, typename ...TArgs>
        RkVoid Execute(TExecutable in_job, TArgs... in_args) noexcept;

        /**
         * \brief Waits for the last task to execute, and starts the execution of this new job 
         * \tparam TExecutable Type of the job
         * \param in_job job to execute when done with the previous one
         * \param in_args Args of the job to execute
         */
        template <typename TExecutable, typename ...TArgs>
        RkVoid ExecuteWithInstance(TExecutable in_job, TArgs... in_args) noexcept;

        /**
         * \brief Locks the current thread until the current job has been done.
         *        If there was no job currently executed, this method has no effect.
         */
        RkVoid WaitForAvailability() noexcept;

        /**
         * \brief Separates the thread of execution from the worker object, allowing execution to continue independently.
         * 
         * \note Any allocated resources will be freed once the thread exits.
         * \note If the worker was available, this method has no effect
         */
        RkVoid Detach() noexcept;

        /**
         * \brief Returns the current underlying thread
         * \return Worker's thread
         */
        [[nodiscard]]
        std::thread& Thread() noexcept;

        [[nodiscard]]
        std::thread::id ID() const noexcept;

        #pragma endregion

        #pragma region Operators

        Worker& operator=(Worker const& in_copy) = delete;
        Worker& operator=(Worker&&      in_move) = default;

        #pragma endregion

};

#include "Threading/Worker.inl"

END_RUKEN_NAMESPACE
