/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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

#include "Build/OperatingSystem.hpp"
#include "Debug/Logging/Formatters/ConsoleFormatter.hpp"

#ifdef RUKEN_OS_WINDOWS

#include "Utility/WindowsOS.hpp"

#define RUKEN_FOREGROUND_GREY   (FOREGROUND_INTENSITY)
#define RUKEN_FOREGROUND_RED    (FOREGROUND_INTENSITY | FOREGROUND_RED)
#define RUKEN_FOREGROUND_GREEN  (FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define RUKEN_FOREGROUND_BLUE   (FOREGROUND_INTENSITY | FOREGROUND_BLUE)
#define RUKEN_FOREGROUND_YELLOW (FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_GREEN)
#define RUKEN_FOREGROUND_PURPLE (FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_BLUE)
#define RUKEN_FOREGROUND_CYAN   (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define RUKEN_FOREGROUND_WHITE  (FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)

#define RUKEN_BACKGROUND_GREY   (BACKGROUND_INTENSITY)
#define RUKEN_BACKGROUND_RED    (BACKGROUND_RED)
#define RUKEN_BACKGROUND_GREEN  (BACKGROUND_GREEN)
#define RUKEN_BACKGROUND_BLUE   (BACKGROUND_BLUE)
#define RUKEN_BACKGROUND_YELLOW (BACKGROUND_RED   | BACKGROUND_GREEN)
#define RUKEN_BACKGROUND_PURPLE (BACKGROUND_RED   | BACKGROUND_BLUE)
#define RUKEN_BACKGROUND_CYAN   (BACKGROUND_GREEN | BACKGROUND_BLUE)
#define RUKEN_BACKGROUND_WHITE  (BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE)

#endif

USING_RUKEN_NAMESPACE

#pragma region Constructors

#ifdef RUKEN_OS_WINDOWS

ConsoleFormatter::ConsoleFormatter() noexcept:
    LogFormatter {},
    m_handle     {GetStdHandle(STD_OUTPUT_HANDLE)}
{
    
}

#endif

#pragma endregion

#pragma region Methods

std::string ConsoleFormatter::ComputeLabel(LogRecord const& in_record) const noexcept
{
    #ifdef RUKEN_OS_WINDOWS

    switch (in_record.level)
    {
        case ELogLevel::Debug:
            SetConsoleTextAttribute(m_handle, RUKEN_FOREGROUND_GREY);
            break;

        case ELogLevel::Info:
            SetConsoleTextAttribute(m_handle, RUKEN_FOREGROUND_WHITE);
            break;

        case ELogLevel::Warning:
            SetConsoleTextAttribute(m_handle, RUKEN_FOREGROUND_YELLOW);
            break;

        case ELogLevel::Error:
            SetConsoleTextAttribute(m_handle, RUKEN_FOREGROUND_RED);
            break;

        case ELogLevel::Fatal:
            SetConsoleTextAttribute(m_handle, RUKEN_FOREGROUND_WHITE | RUKEN_BACKGROUND_RED);
            break;
    }

    #endif

    return LogFormatter::ComputeLabel(in_record);
}

#pragma endregion