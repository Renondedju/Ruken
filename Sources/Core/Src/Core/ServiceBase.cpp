
#include "Core/ServiceBase.hpp"

USING_RUKEN_NAMESPACE

RkSize ServiceBase::GetNextId() noexcept
{
    static RkSize id = 0;

    return id++;
}

RkVoid ServiceBase::SignalServiceInitializationFailure(std::string&& in_reason) noexcept
{
    m_status         = EInitializationStatus::Failed;
    m_failure_reason = std::forward<std::string>(in_reason);
}

EInitializationStatus ServiceBase::CheckInitializationStatus(std::string& out_reason) const noexcept
{
    out_reason = m_failure_reason;

    return m_status;
}