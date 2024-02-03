#pragma once

#pragma region Lifetime

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>::CPUAwaitableHandle(nullptr_t) noexcept:
    m_instance {nullptr}
{}

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>::CPUAwaitableHandle(CPUAwaitable<TResult, TNoexcept>& in_awaitable) noexcept:
    m_instance {std::addressof(in_awaitable)}
{
    m_instance->IncrementReferenceCount();
}

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>::CPUAwaitableHandle(CPUAwaitableHandle const& in_other) noexcept:
    m_instance {in_other.m_instance}
{
    if (m_instance) 
        m_instance->IncrementReferenceCount();
}

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>::CPUAwaitableHandle(CPUAwaitableHandle&& in_other) noexcept:
    m_instance {in_other.m_instance}
{
    if (m_instance) 
        m_instance->IncrementReferenceCount();
}

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>& CPUAwaitableHandle<TResult, TNoexcept>::operator=(CPUAwaitableHandle const& in_other) noexcept
{
    if (m_instance) m_instance->DecrementReferenceCount();
    m_instance = in_other.m_instance;
    if (m_instance) m_instance->IncrementReferenceCount();

    return *this;
}

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>& CPUAwaitableHandle<TResult, TNoexcept>::operator=(CPUAwaitableHandle&& in_other) noexcept
{
    if (m_instance) m_instance->DecrementReferenceCount();
    m_instance = std::move(in_other.m_instance);
    if (m_instance) m_instance->IncrementReferenceCount();

    return *this;
}

template <typename TResult, RkBool TNoexcept>
CPUAwaitableHandle<TResult, TNoexcept>::~CPUAwaitableHandle() noexcept
{
    if (m_instance)
        m_instance->DecrementReferenceCount();
}

#pragma endregion

#pragma region Methods

template <typename TResult, RkBool TNoexcept>
std::add_lvalue_reference_t<const TResult> CPUAwaitableHandle<TResult, TNoexcept>::GetResult() const noexcept
	requires !std::is_same_v<TResult, RkVoid>
{
    return m_instance->GetResult();
}

template <typename TResult, RkBool TNoexcept>
std::exception_ptr CPUAwaitableHandle<TResult, TNoexcept>::GetException() const noexcept requires !TNoexcept
{
    return m_instance->GetException();
}

#pragma endregion
