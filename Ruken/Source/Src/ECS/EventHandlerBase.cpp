
#include "ECS/EventHandlerBase.hpp"

USING_RUKEN_NAMESPACE

ComponentQuery const& EventHandlerBase::GetQuery() const noexcept
{
    return m_query;
}