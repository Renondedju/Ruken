#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUPropagatingContinuation.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult, RkBool TNoexcept>
struct CPUCoroutineContinuation: CPUPropagatingContinuation<TResult, TNoexcept>
{
    using Node = typename CPUPropagatingContinuation<TResult, TNoexcept>::Node;

    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_coroutine Owning coroutine instance
     * \param in_awaited Reference to the awaited event
     */
    CPUCoroutineContinuation(CPUAwaiter& in_coroutine, CPUAwaitableHandle<TResult, TNoexcept>&& in_awaited) noexcept
    { this->Setup(in_coroutine, std::forward<CPUAwaitableHandle<TResult, TNoexcept>>(in_awaited)); }

    CPUCoroutineContinuation(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation(CPUCoroutineContinuation&&     ) = delete;
    ~CPUCoroutineContinuation() noexcept
    { this->Detach(); }

    CPUCoroutineContinuation operator=(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation operator=(CPUCoroutineContinuation&&     ) = delete;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool await_ready() const noexcept
    { return this->IsEventCompleted(); }

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool await_suspend(std::coroutine_handle<>) noexcept
    { return this->TryAttach(); }

    /**
     * \brief Returns the result of the wait
     */
    auto await_resume() const noexcept(TNoexcept)
    {
        if constexpr (TNoexcept == false)
			if (this->GetException())
				std::rethrow_exception(this->GetException());

	    return this->GetReturnValue();
    }

    #pragma endregion
};

template <RkBool TNoexcept>
struct CPUCoroutineContinuation<RkVoid, TNoexcept>: CPUPropagatingContinuation<RkVoid, TNoexcept>
{
    #pragma region Lifetime

    /**
     * \brief Default constructor
     * \param in_coroutine Owning coroutine instance
     * \param in_awaited Handle to the awaited event
     */
    CPUCoroutineContinuation(CPUAwaiter& in_coroutine, CPUAwaitableHandle<RkVoid, TNoexcept>&& in_awaited) noexcept
    { this->Setup(in_coroutine, std::forward<CPUAwaitableHandle<RkVoid, TNoexcept>>(in_awaited)); }

    CPUCoroutineContinuation(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation(CPUCoroutineContinuation&&     ) = delete;
    ~CPUCoroutineContinuation() noexcept
    { this->Detach(); }

    CPUCoroutineContinuation operator=(CPUCoroutineContinuation const&) = delete;
    CPUCoroutineContinuation operator=(CPUCoroutineContinuation&&     ) = delete;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Checks if the wait has been completed already
     * \note This function can be called even if the awaited event has been deleted already
     * \return True if the awaiter has been completed, false otherwise
     */
    [[nodiscard]]
	RkBool await_ready() const noexcept
    { return this->IsEventCompleted(); }

    /**
     * \brief Attempts a suspension by attaching the awaiter to the awaited event
     * \return True if the suspension succeeded, false otherwise
     */
    [[nodiscard]]
    RkBool await_suspend(std::coroutine_handle<>) noexcept
    { return this->TryAttach(); }

    /**
     * \brief Returns the result of the wait
     */
    RkVoid await_resume() const noexcept(TNoexcept)
    {
        if constexpr (TNoexcept == false)
			if (this->GetException())
				std::rethrow_exception(this->GetException());
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE
