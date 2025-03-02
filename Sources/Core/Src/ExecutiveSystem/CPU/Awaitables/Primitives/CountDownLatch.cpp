#include "ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp"

#include <tracy/Tracy.hpp>

USING_RUKEN_NAMESPACE

CountDownLatch::CountDownLatch(RkSize const in_initial_count) noexcept:
    ManualResetEvent {},
	m_counter        {in_initial_count}
{}

RkVoid CountDownLatch::Signal() noexcept
{
    if (m_counter.fetch_sub(1, std::memory_order_acq_rel) == 1ULL)
		SignalConsume();
}

RkSize CountDownLatch::Value() const noexcept
{
    return m_counter.load(std::memory_order_relaxed);
}