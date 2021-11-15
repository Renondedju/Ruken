
#include "ECS/SystemBase.hpp"

USING_RUKEN_NAMESPACE

ComponentQuery const& SystemBase::GetQuery() const noexcept
{
    return m_query;
}

RkVoid SystemBase::OnStart() noexcept
{}

RkVoid SystemBase::OnUpdate() noexcept
{}

RkVoid SystemBase::OnEnd() noexcept
{}
