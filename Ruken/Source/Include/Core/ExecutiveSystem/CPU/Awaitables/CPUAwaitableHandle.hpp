#pragma once

BEGIN_RUKEN_NAMESPACE

template <typename TReturnType>
class CPUAwaitable;
struct CPUContinuation;

/**
 * \brief Some CPU Awaitables can hold results, preventing them from cleaning up
 *        upon completion. The CPUAwaitableHandle is responsible for that instead.
 *        The class is a handle that updates an atomic counter based on its own lifetime.
 *        Retrieving the result of an awaitable is only possible by creating a
 *        suspension via the handle, ensuring that the awaitable will stay alive as long
 *        as a reference to it is held.
 *
 * \tparam TReturnType The return type of the refereed awaitable
 * \note   This class has no side effects when TReturnType is void
 */
template <typename TReturnType>
class CPUAwaitableHandle
{
    friend CPUContinuation;

    CPUAwaitable<TReturnType>* m_instance;

    public:

        #pragma region Lifetime

        explicit CPUAwaitableHandle(nullptr_t)                               noexcept;
        explicit CPUAwaitableHandle(CPUAwaitable<TReturnType>& in_awaitable) noexcept;

        CPUAwaitableHandle (CPUAwaitableHandle const&) noexcept;
        CPUAwaitableHandle (CPUAwaitableHandle&&)      noexcept;
        ~CPUAwaitableHandle()                          noexcept;

        CPUAwaitableHandle& operator=(CPUAwaitableHandle const&) noexcept;
        CPUAwaitableHandle& operator=(CPUAwaitableHandle&&)      noexcept;

        #pragma endregion

        #pragma region Methods

        using Return = std::conditional_t<std::is_same_v<TReturnType, RkVoid>, RkInt, TReturnType> const&;

        /**
         * \brief Returns the result of the awaitable.
         * \warning Do note that this result is valid only if the awaitable has been completed.
         * \return Const reference to the result value
         */
        template <typename = std::enable_if_t<!std::is_same_v<TReturnType, RkVoid>>>
        [[nodiscard]] Return GetResult() const noexcept;

        #pragma endregion
};

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.inl"

END_RUKEN_NAMESPACE
