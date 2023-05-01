#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements the base common behavior for all CPU tasks
 * \tparam TReturnType Return type of the associated coroutine
 */
template <typename TReturnType>
class CPUPromise: public CPUAwaitable
{
    #pragma region Members

    TReturnType         m_result;
    std::atomic<RkSize> m_references;

    #pragma endregion

    struct FinalSuspension {
        CPUPromise& owner;

        [[nodiscard]] RkBool await_ready() const noexcept {
            // Automatically destroy the coroutine frame if no references are currently held
            return owner.m_references.load(std::memory_order_acquire) == 0;
        }

        constexpr RkVoid await_suspend(std::coroutine_handle<>) const noexcept {}
        constexpr RkVoid await_resume ()                        const noexcept {}
    };

    public:

        using ProcessingUnit = CentralProcessingUnit;

        #pragma region Methods

        /**
         * \brief Signals to the promise that a reference has been made to it
         *        Preventing if from destroying itself before the result could have been read.
         */
        RkVoid AddReference() noexcept
        {
            m_references.fetch_add(1, std::memory_order_release);
        }
        
        /**
         * \brief 
         */
        RkVoid RemoveReference() noexcept
        {
            // If the last reference has been dropped and the task is completed, then we need to manually
            // destroy the allocated frame.
            if (m_references.fetch_sub(1, std::memory_order_acq_rel) == 1 && this->Completed())
                std::coroutine_handle<CPUPromise>::from_promise(*this).destroy();
        }

        /// ----- Coroutine methods -----
        ///

        /**
         * \brief Called when the coroutine returns a value
         * \param in_result Result emitted by the coroutine
         */
        RkVoid return_value(TReturnType& in_result) const noexcept
        { m_result = std::forward<TReturnType>(in_result); }

        // CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed.
        // Final suspension depends on the number of references that are made to the coroutine.
        // Since we have to hold a result, the promise cannot be destroyed if there are still references to it
        // in that case, the last reference to be removed will destroy the coroutine.
        // If no references are made to the coroutine at the time of completion, the destruction happens immediately.
        auto initial_suspend() noexcept {                           return std::suspend_always {};      }
        auto final_suspend  () noexcept { this->SignalCompletion(); return FinalSuspension     {*this}; }

        #pragma endregion
};

/**
 * \brief Implements the base common behavior for all CPU tasks
 */
template <>
class CPUPromise<RkVoid>: CPUAwaitable
{
    public:

        // A promise with a return type of void will return nothing
        // thanks to that, this specialization can get rid of the reference tracking
        // and destroy itself as soon as final_suspend has been called
        void return_void() const noexcept {}

        // CPU tasks will never start synchronously and are instead inserted into queues for it to be eventually processed. 
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never  final_suspend  () noexcept { this->SignalCompletion(); return {}; }
};

END_RUKEN_NAMESPACE