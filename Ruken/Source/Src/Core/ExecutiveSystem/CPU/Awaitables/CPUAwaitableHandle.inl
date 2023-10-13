#pragma once

#pragma region Lifetime

inline CPUAwaitableHandle<RkVoid>::CPUAwaitableHandle(nullptr_t) noexcept:
    m_instance {nullptr}
{}

inline CPUAwaitableHandle<RkVoid>::CPUAwaitableHandle(CPUAwaitable<RkVoid>& in_awaitable) noexcept:
    m_instance {std::addressof(in_awaitable)}
{}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(nullptr_t) noexcept:
    m_instance {nullptr}
{}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitable<TReturnType>& in_awaitable) noexcept:
    m_instance {std::addressof(in_awaitable)}
{
    m_instance->IncrementReferenceCount();
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitableHandle const& in_other) noexcept:
    m_instance {in_other.m_instance}
{
    m_instance->IncrementReferenceCount();
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::CPUAwaitableHandle(CPUAwaitableHandle&& in_other) noexcept:
    m_instance {in_other.m_instance}
{
    m_instance->IncrementReferenceCount();
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>& CPUAwaitableHandle<TReturnType>::operator=(CPUAwaitableHandle const& in_other) noexcept
{
    m_instance->DecrementReferenceCount();
    m_instance = in_other.m_instance;
    m_instance->IncrementReferenceCount();

    return *this;
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>& CPUAwaitableHandle<TReturnType>::operator=(CPUAwaitableHandle&& in_other) noexcept
{
    m_instance->DecrementReferenceCount();
    m_instance = std::move(in_other.m_instance);
    m_instance->IncrementReferenceCount();

    return *this;
}

template <typename TReturnType>
CPUAwaitableHandle<TReturnType>::~CPUAwaitableHandle() noexcept
{ RkBool orphaned; m_instance->DecrementReferenceCount(true, orphaned); }

#pragma endregion

#pragma region Methods

template <typename TReturnType>
TReturnType const& CPUAwaitableHandle<TReturnType>::GetResult() const noexcept
{
    return m_instance->GetResult();
}

#pragma endregion
