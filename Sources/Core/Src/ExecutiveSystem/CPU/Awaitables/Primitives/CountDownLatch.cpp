#include "ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp"

USING_RUKEN_NAMESPACE

CountDownLatch::CountDownLatch(RkSize const in_initial_count) noexcept:
    CPUAwaitable {m_continuation_node},
	m_counter    {in_initial_count}
{}

RkVoid CountDownLatch::Signal() noexcept
{
    if (m_counter.fetch_sub(1, std::memory_order_acq_rel) == 1ULL)
		Consume(true);
}

RkSize CountDownLatch::Value() const noexcept
{
    return m_counter.load(std::memory_order_relaxed);
}