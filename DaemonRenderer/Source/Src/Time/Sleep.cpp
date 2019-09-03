/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <chrono>
#include <thread>

#include "Time/Sleep.hpp"
#include "Utility/WindowsOS.hpp"

USING_DAEMON_NAMESPACE

DAEbool Sleep::NsSleep(DAEint64 const in_nanoseconds) noexcept
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

DAEvoid Sleep::StdSleep(DAEdouble const in_seconds) noexcept
{
    static constexpr std::chrono::duration<DAEdouble> min_sleep_duration(0);
    std::chrono::steady_clock::time_point const start = std::chrono::steady_clock::now();
    while (std::chrono::duration<DAEdouble>(std::chrono::steady_clock::now() - start).count() < in_seconds)
        std::this_thread::sleep_for(min_sleep_duration);
}
