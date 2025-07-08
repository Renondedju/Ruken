#include "JobSystem/Awaitables/Awaitable.hpp"
#include "JobSystem/Awaitables/Awaiter.hpp"

#include "../../Debug/Assert.hpp"

USING_RUKEN_NAMESPACE

Awaiter Awaitable::operator co_await() const noexcept
{
    return Awaiter(&m_awaiter_list);
}
