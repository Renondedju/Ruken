/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

        
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
