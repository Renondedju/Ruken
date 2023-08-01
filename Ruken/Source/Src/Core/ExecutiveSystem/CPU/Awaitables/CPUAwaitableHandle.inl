#pragma once

#pragma region Lifetime

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitable<TReturnType>& in_handle) noexcept:
    m_handle {in_handle}
{
    m_handle.m_references.fetch_add(1, std::memory_order_release);
}

inline CPUAwaitableHandle<RkVoid>::CPUAwaitableHandle(CPUAwaitable<RkVoid>& in_handle) noexcept:
    m_handle {in_handle}
{}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitableHandle const& in_other) noexcept:
    m_handle {in_other.m_handle}
{
    m_handle.m_references.fetch_add(1, std::memory_order_release);
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitableHandle&& in_other) noexcept:
    m_handle {in_other.m_handle}
{
    m_handle.m_references.fetch_add(1, std::memory_order_release);
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::~CPUAwaitableHandle() noexcept
{
    if (m_handle.m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
        m_handle.NotifyOrphanedAwaitable();
}

#pragma endregion

#pragma region Methods

template <typename TReturnType>
CPUReturningSuspension<TReturnType> CPUAwaitableHandle<TReturnType>::GetSuspension(CPUAwaitableCallback& in_completion_callback) noexcept
{
    return CPUReturningSuspension<TReturnType> {m_handle.m_suspension_node, in_completion_callback, m_handle.m_result};
}

inline CPUSuspension CPUAwaitableHandle<RkVoid>::GetSuspension(CPUAwaitableCallback& in_completion_callback) const noexcept
{
    return CPUSuspension {
        .head                = m_handle.m_suspension_node,
        .completion_callback = in_completion_callback
    };
}

#pragma endregion