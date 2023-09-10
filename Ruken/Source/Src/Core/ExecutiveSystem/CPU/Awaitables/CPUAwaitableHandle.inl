#pragma once

#pragma region Lifetime

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitable<TReturnType>& in_handle) noexcept:
    m_handle {in_handle}
{
    if constexpr(!std::is_same_v<TReturnType, RkVoid>)
        m_handle.m_references.fetch_add(1, std::memory_order_release);
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitableHandle const& in_other) noexcept:
    m_handle {in_other.m_handle}
{
    if constexpr(!std::is_same_v<TReturnType, RkVoid>)
        m_handle.m_references.fetch_add(1, std::memory_order_release);
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitableHandle&& in_other) noexcept:
    m_handle {in_other.m_handle}
{
    if constexpr(!std::is_same_v<TReturnType, RkVoid>)
        m_handle.m_references.fetch_add(1, std::memory_order_release);
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::~CPUAwaitableHandle() noexcept
{
    if constexpr(!std::is_same_v<TReturnType, RkVoid>)
    {
        if (m_handle.m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
            m_handle.NotifyOrphanedAwaitable();    
    }
}

#pragma endregion