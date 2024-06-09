
        
template<class TData, EAccessMode TMode>
SynchronizedAccess<TData, TMode>::SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept:
    m_synchronized {in_synchronized}
{}

template <class TData, EAccessMode TMode>
TData& SynchronizedAccess<TData, TMode>::Get() noexcept
{
    return m_synchronized.m_value;
}

template <class TData, EAccessMode TMode>
TData const& SynchronizedAccess<TData, TMode>::Get() const noexcept
{
    return m_synchronized.m_value;
}

template <class TData, EAccessMode TMode>
TData& SynchronizedAccess<TData, TMode>::operator*() noexcept
{
    return m_synchronized.m_value;
}

template <class TData, EAccessMode TMode>
TData const& SynchronizedAccess<TData, TMode>::operator*() const noexcept
{
    return m_synchronized.m_value;
}

template <class TData, EAccessMode TMode>
TData* SynchronizedAccess<TData, TMode>::operator->() noexcept
{
    return &m_synchronized.m_value;
}

template <class TData, EAccessMode TMode>
TData const* SynchronizedAccess<TData, TMode>::operator->() const noexcept
{
    return &m_synchronized.m_value;
}

template<class TData>
SynchronizedAccess<TData, EAccessMode::Read>::SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept:
    m_synchronized {in_synchronized},
    m_lock         {in_synchronized.m_mutex}
{}

template<class TData>
std::shared_lock<std::shared_mutex>& SynchronizedAccess<TData, EAccessMode::Read>::GetLock() noexcept
{
    return m_lock;
}

template <class TData>
TData const& SynchronizedAccess<TData, EAccessMode::Read>::Get() const noexcept
{
    return m_synchronized.m_value;
}

template <class TData>
TData const& SynchronizedAccess<TData, EAccessMode::Read>::operator*() const noexcept
{
    return m_synchronized.m_value;
}

template <class TData>
TData const* SynchronizedAccess<TData, EAccessMode::Read>::operator->() const noexcept
{
    return &m_synchronized.m_value;
}

template<class TData>
SynchronizedAccess<TData, EAccessMode::Write>::SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept:
    m_synchronized {in_synchronized},
    m_lock         {in_synchronized.m_mutex}
{}

template<class TData>
std::unique_lock<std::shared_mutex>& SynchronizedAccess<TData, EAccessMode::Write>::GetLock() noexcept
{
    return m_lock;
}

template <class TData>
TData& SynchronizedAccess<TData, EAccessMode::Write>::Get() noexcept
{
    return m_synchronized.m_value;
}

template <class TData>
TData const& SynchronizedAccess<TData, EAccessMode::Write>::Get() const noexcept
{
    return m_synchronized.m_value;
}

template <class TData>
TData& SynchronizedAccess<TData, EAccessMode::Write>::operator*() noexcept
{
    return m_synchronized.m_value;
}

template <class TData>
TData const& SynchronizedAccess<TData, EAccessMode::Write>::operator*() const noexcept
{
    return m_synchronized.m_value;
}

template <class TData>
TData* SynchronizedAccess<TData, EAccessMode::Write>::operator->() noexcept
{
    return &m_synchronized.m_value;
}

template <class TData>
TData const* SynchronizedAccess<TData, EAccessMode::Write>::operator->() const noexcept
{
    return &m_synchronized.m_value;
}
