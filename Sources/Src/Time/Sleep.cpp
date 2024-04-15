
#include <chrono>
#include <thread>

#include "Time/Sleep.hpp"
#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

RkBool Sleep::NsSleep(RkInt64 const in_nanoseconds) noexcept
{
    HANDLE const timer = CreateWaitableTimer(nullptr, true, nullptr);
    LARGE_INTEGER li;

    if(!timer)
        return false;

    li.QuadPart = -in_nanoseconds;
    if(!SetWaitableTimer(timer, &li, 0, nullptr, nullptr, false))
    {
        CloseHandle(timer);
        return false;
    }

    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return true;
}

RkVoid Sleep::StdSleep(RkDouble const in_seconds) noexcept
{
    static constexpr std::chrono::duration<RkDouble> min_sleep_duration(0);
    std::chrono::steady_clock::time_point const start = std::chrono::steady_clock::now();
    while (std::chrono::duration<RkDouble>(std::chrono::steady_clock::now() - start).count() < in_seconds)
        std::this_thread::sleep_for(min_sleep_duration);
}
