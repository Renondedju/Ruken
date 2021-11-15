
#include "Time/Timer.hpp"
#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

Timer::Timer() noexcept:
    m_win_handle {CreateWaitableTimer(nullptr, true, nullptr)}
{}

Timer::~Timer() noexcept
{
    CloseHandle(static_cast<HANDLE>(m_win_handle));
}

RkBool Timer::SetTiming(RkInt64 const in_nanoseconds) const noexcept
{
    if(!m_win_handle)
        return false;

    LARGE_INTEGER li;
    li.QuadPart = -in_nanoseconds;

    return SetWaitableTimer(m_win_handle, &li, 0, nullptr, nullptr, false);
}

RkBool Timer::NSleep() const noexcept
{
    if(!m_win_handle)
        return false;

    WaitForSingleObject(m_win_handle, INFINITE);

    return true;
}
