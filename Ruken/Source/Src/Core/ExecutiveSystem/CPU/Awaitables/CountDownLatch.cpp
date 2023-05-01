#include "Core/ExecutiveSystem/CPU/Awaitables/CountDownLatch.hpp"

USING_RUKEN_NAMESPACE

CountDownLatch::CountDownLatch(RkSize const in_initial_count) noexcept:
	m_count {in_initial_count}
{}

RkVoid CountDownLatch::CountDown(RkSize const in_amount) noexcept
{
	if (m_count.fetch_sub(in_amount, std::memory_order_release) - in_amount == 0ULL)
		SignalCompletion();
}
