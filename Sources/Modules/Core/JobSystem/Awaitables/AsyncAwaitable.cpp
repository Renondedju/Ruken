#include "JobSystem/Awaitables/AsyncAwaitable.hpp"
#include "JobSystem/Awaitables/AsyncAwaiter.hpp"

USING_RUKEN_NAMESPACE

AsyncAwaiter AsyncAwaitable::operator co_await() const noexcept
{
    return AsyncAwaiter(&m_awaiter_list);
}