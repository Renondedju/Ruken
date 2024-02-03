#pragma once

#include <exception>

BEGIN_RUKEN_NAMESPACE

template <typename TResult, RkBool TNoexcept>
class  CPUAwaitable;
struct CPUContinuation;

/**
 * \brief Some CPU Awaitables can hold results, preventing them from cleaning up
 *        upon completion. The CPUAwaitableHandle is responsible for that instead.
 *        The class is a handle that updates an atomic counter based on its own lifetime.
 *        Retrieving the result of an awaitable is only possible by creating a
 *        suspension via the handle, ensuring that the awaitable will stay alive as long
 *        as a reference to it is held.
 *
 * \tparam TResult The result of the refereed awaitable
 * \tparam TNoexcept 
 * \note   This class has no side effects when TResult is void
 */
template <typename TResult, RkBool TNoexcept = true>
class CPUAwaitableHandle
{
    friend CPUContinuation;

    CPUAwaitable<TResult, TNoexcept>* m_instance;

    public:

        #pragma region Lifetime

        explicit CPUAwaitableHandle(nullptr_t)                                      noexcept;
        explicit CPUAwaitableHandle(CPUAwaitable<TResult, TNoexcept>& in_awaitable) noexcept;

        CPUAwaitableHandle (CPUAwaitableHandle const&) noexcept;
        CPUAwaitableHandle (CPUAwaitableHandle&&)      noexcept;
        ~CPUAwaitableHandle()                          noexcept;

        CPUAwaitableHandle& operator=(CPUAwaitableHandle const&) noexcept;
        CPUAwaitableHandle& operator=(CPUAwaitableHandle&&)      noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the result of the awaitable.
         * \warning Do note that this result is valid only if the awaitable has been completed.
         * \return Const reference to the result value
         */
        [[nodiscard]] std::add_lvalue_reference_t<const TResult> GetResult() const noexcept
			requires !std::is_same_v<TResult, RkVoid>;

        /**
         * \brief Returns the result of the awaitable.
         * \warning Do note that this result is valid only if the awaitable has been completed.
         * \return Const reference to the result value
         */
        [[nodiscard]] std::exception_ptr GetException() const noexcept requires !TNoexcept;

        #pragma endregion
};

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.inl"

END_RUKEN_NAMESPACE
