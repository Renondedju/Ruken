#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUReturningContinuation.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TReturnType>
struct CPUCoroutineContinuation: CPUReturningContinuation<TReturnType>
{
    using Node = typename CPUReturningContinuation<TReturnType>::Node;

    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_return_value Reference to the return value
     * \param in_hook Continuation hook of the CPUAwaitable
     * \param in_coroutine Coroutine instance
     */
    CPUCoroutineContinuation(TReturnType& in_return_value, Node& in_hook, CPUAwaiter& in_coroutine) noexcept:
        CPUReturningContinuation<TReturnType>::return_value {in_return_value}
    {
        CPUReturningContinuation<TReturnType>::hook  = std::addressof(in_hook);
        CPUReturningContinuation<TReturnType>::owner = std::addressof(in_coroutine);
    }

    CPUCoroutineContinuation(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation(CPUCoroutineContinuation&&     ) = delete;
    ~CPUCoroutineContinuation() noexcept
    { CPUReturningContinuation<TReturnType>::Detach(); }

    CPUCoroutineContinuation operator=(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation operator=(CPUCoroutineContinuation&&     ) = delete;

    #pragma region

    #pragma region Methods

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool await_ready() const noexcept
    { return CPUReturningContinuation<TReturnType>::IsEventCompleted(); }

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool await_suspend(std::coroutine_handle<>) noexcept
    { return CPUReturningContinuation<TReturnType>::TryAttach(); }

    /**
     * \brief Returns the result of the wait
     */
    TReturnType const& await_resume() const noexcept
    { return CPUReturningContinuation<TReturnType>::return_value; }

    #pragma region
};

template <>
struct CPUCoroutineContinuation<RkVoid>: CPUContinuation
{
    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_hook Continuation hook of the CPUAwaitable
     * \param in_coroutine Coroutine instance
     */
    CPUCoroutineContinuation(Node& in_hook, CPUAwaiter& in_coroutine) noexcept
    {
        hook  = std::addressof(in_hook);
        owner = std::addressof(in_coroutine);
    }

    CPUCoroutineContinuation(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation(CPUCoroutineContinuation&&     ) = delete;
    ~CPUCoroutineContinuation() noexcept
    { Detach(); }

    CPUCoroutineContinuation operator=(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation operator=(CPUCoroutineContinuation&&     ) = delete;

    #pragma region

    #pragma region Methods

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool await_ready() const noexcept
    { return IsEventCompleted(); }

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool await_suspend(std::coroutine_handle<>) noexcept
    { return TryAttach(); }

    /**
     * \brief Returns the result of the wait
     */
    static constexpr RkVoid await_resume() noexcept { }

    #pragma region
};

END_RUKEN_NAMESPACE
