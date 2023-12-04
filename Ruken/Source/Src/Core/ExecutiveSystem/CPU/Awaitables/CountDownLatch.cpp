#include "Core/ExecutiveSystem/CPU/Awaitables/CountDownLatch.hpp"

USING_RUKEN_NAMESPACE

CountDownLatch::CountDownLatch(RkSize const in_initial_count) noexcept:
	m_count {in_initial_count}
{}

RkVoid CountDownLatch::CountDown() noexcept
{
    if (m_count.fetch_sub(1, std::memory_order_acq_rel) == 1ULL)
		SignalConsume();
}

RkSize CountDownLatch::Value() const noexcept
{
    return m_count.load(std::memory_order_relaxed);
}

RkVoid CountDownLatch::OnAwaitedContinuation() noexcept
{
    CountDown();
}
