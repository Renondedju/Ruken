#include "ExecutiveSystem/Awaitables/Awaitable.hpp"
#include "ExecutiveSystem/Awaitables/Awaiter.hpp"

#include "Meta/Assert.hpp"

USING_RUKEN_NAMESPACE

Awaiter Awaitable::operator co_await() const noexcept
{
    return Awaiter(&m_awaiter_list);
}
