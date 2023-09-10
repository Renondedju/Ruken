#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

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
    public:
    CPUAwaitable<TReturnType>& m_handle;

    public:

        #pragma region Lifetime

        explicit CPUAwaitableHandle(CPUAwaitable<TReturnType>& in_handle) noexcept;
        CPUAwaitableHandle (CPUAwaitableHandle const&) noexcept;
        CPUAwaitableHandle (CPUAwaitableHandle&&)      noexcept;
        ~CPUAwaitableHandle() noexcept;

        CPUAwaitableHandle& operator=(CPUAwaitableHandle const&) = delete;
        CPUAwaitableHandle& operator=(CPUAwaitableHandle&&)      = delete;

        #pragma endregion
};

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitableHandle.inl"

END_RUKEN_NAMESPACE
