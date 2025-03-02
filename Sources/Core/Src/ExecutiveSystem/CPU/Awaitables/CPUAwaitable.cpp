#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"

#include "Meta/Assert.hpp"

USING_RUKEN_NAMESPACE

CPUAwaiter CPUAwaitable::operator co_await() const noexcept
{
    return CPUAwaiter(&m_awaiter_list);
}
