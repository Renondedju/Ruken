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
     * \param in_coroutine Owning coroutine instance
     * \param in_awaited Reference to the awaited event
     */
    CPUCoroutineContinuation(CPUAwaiter& in_coroutine, CPUAwaitableHandle<TReturnType>&& in_awaited) noexcept
    { CPUReturningContinuation<TReturnType>::Setup(in_coroutine, std::forward<CPUAwaitableHandle<RkVoid>>(in_awaited)); }

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
    { return CPUReturningContinuation<TReturnType>::GetReturnValue(); }

    #pragma region
};

template <>
struct CPUCoroutineContinuation<RkVoid>: CPUContinuation
{
    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_coroutine Owning coroutine instance
     * \param in_awaited Handle to the awaited event
     */
    CPUCoroutineContinuation(CPUAwaiter& in_coroutine, CPUAwaitableHandle<RkVoid>&& in_awaited) noexcept
    { Setup(in_coroutine, std::forward<CPUAwaitableHandle<RkVoid>>(in_awaited)); }

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
    constexpr RkVoid await_resume() noexcept { }

    #pragma region
};

END_RUKEN_NAMESPACE
