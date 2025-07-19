#pragma once

USING_RUKEN_NAMESPACE

template<typename TValue>
template<typename ... TArgs> requires std::is_constructible_v<TValue, TArgs...>
Synchronized<TValue>::Synchronized(TArgs&&... in_args) noexcept(noexcept(TValue(in_args...))):
    m_value {std::forward<TArgs>(in_args)...}
{}

template <typename TValue>
Synchronized<TValue>::Synchronized(Synchronized const& in_copy) noexcept:
    m_value {in_copy.m_value}
{}

template <typename TValue>
Synchronized<TValue>::Synchronized(Synchronized&& in_move) noexcept:
    m_value {std::forward<TValue>(in_move.m_value)}
{}

template <typename TValue>
TValue const& Synchronized<TValue>::Unsafe() const noexcept
{
    return m_value;
}

template <typename TValue>
TValue& Synchronized<TValue>::Unsafe() noexcept
{
    return m_value;
}

template<typename TValue>
typename Synchronized<TValue>::ReadAccess Synchronized<TValue>::Read() noexcept
{
    return ReadAccess{*this};
}

template<typename TValue>
typename Synchronized<TValue>::WriteAccess Synchronized<TValue>::Write() noexcept
{
    return WriteAccess {*this};
}


