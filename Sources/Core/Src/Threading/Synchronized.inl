
template <typename TType>
template <typename ...TArgs, typename>
Synchronized<TType>::Synchronized(TArgs... in_args):
    m_mutex {},
    m_value {std::forward<TArgs>(in_args)...}
{}

template <typename TType>
Synchronized<TType>::Synchronized() noexcept:
    m_mutex {},
    m_value {}
{}

template <typename TType>
Synchronized<TType>::Synchronized(Synchronized const& in_copy) noexcept:
    m_mutex {},
    m_value {in_copy.m_value}
{}

template <typename TType>
Synchronized<TType>::Synchronized(Synchronized&& in_move) noexcept:
    m_mutex {},
    m_value {std::forward(in_move.m_value)}
{}

template <typename TType>
TType const& Synchronized<TType>::Unsafe() const noexcept
{
    return m_value;
}

template <typename TType>
TType& Synchronized<TType>::Unsafe() noexcept
{
    return m_value;
}