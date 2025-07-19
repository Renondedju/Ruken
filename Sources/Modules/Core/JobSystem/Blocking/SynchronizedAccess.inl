#pragma once

#include "JobSystem/Blocking/SynchronizedAccess.hpp"
#include "JobSystem/Blocking/Synchronized.hpp"

USING_RUKEN_NAMESPACE

template<typename TValue>
SynchronizedAccess<TValue>::SynchronizedAccess(Synchronized<std::remove_cv_t<TValue>>& in_value) noexcept:
	m_value {in_value}
{
	if constexpr (std::is_const_v<TValue>)
		m_value.m_mutex.lock_shared();
	else
		m_value.m_mutex.lock();
}

template<typename TValue>
SynchronizedAccess<TValue>::~SynchronizedAccess() noexcept
{
	if constexpr (std::is_const_v<TValue>)
		m_value.m_mutex.unlock_shared();
	else
		m_value.m_mutex.unlock();
}

template<typename TValue>
TValue& SynchronizedAccess<TValue>::Get() const noexcept
{
	return m_value.m_value;
}

template<typename TValue>
TValue& SynchronizedAccess<TValue>::operator*() const noexcept
{
	return m_value.m_value;
}

template<typename TValue>
TValue* SynchronizedAccess<TValue>::operator->() const noexcept
{
	return &m_value.m_value;
}
