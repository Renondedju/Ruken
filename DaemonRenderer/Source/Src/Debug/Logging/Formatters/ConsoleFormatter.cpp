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

#include "Debug/Logging/Formatters/ConsoleFormatter.hpp"

#ifdef DAEMON_OS_WINDOWS
    #define DAEMON_FOREGROUND_GREY      (FOREGROUND_INTENSITY)
    #define DAEMON_FOREGROUND_RED       (FOREGROUND_INTENSITY | FOREGROUND_RED)
    #define DAEMON_FOREGROUND_GREEN     (FOREGROUND_INTENSITY | FOREGROUND_GREEN)
    #define DAEMON_FOREGROUND_BLUE      (FOREGROUND_INTENSITY | FOREGROUND_BLUE)
    #define DAEMON_FOREGROUND_YELLOW    (FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_GREEN)
    #define DAEMON_FOREGROUND_PURPLE    (FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_BLUE)
    #define DAEMON_FOREGROUND_CYAN      (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)
    #define DAEMON_FOREGROUND_WHITE     (FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)

    #define DAEMON_BACKGROUND_GREY      (BACKGROUND_INTENSITY)
    #define DAEMON_BACKGROUND_RED       (BACKGROUND_RED)
    #define DAEMON_BACKGROUND_GREEN     (BACKGROUND_GREEN)
    #define DAEMON_BACKGROUND_BLUE      (BACKGROUND_BLUE)
    #define DAEMON_BACKGROUND_YELLOW    (BACKGROUND_RED   | BACKGROUND_GREEN)
    #define DAEMON_BACKGROUND_PURPLE    (BACKGROUND_RED   | BACKGROUND_BLUE)
    #define DAEMON_BACKGROUND_CYAN      (BACKGROUND_GREEN | BACKGROUND_BLUE)
    #define DAEMON_BACKGROUND_WHITE     (BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE)
#else
    #define DAEMON_FOREGROUND_GREY      
    #define DAEMON_FOREGROUND_RED       
    #define DAEMON_FOREGROUND_GREEN     
    #define DAEMON_FOREGROUND_BLUE      
    #define DAEMON_FOREGROUND_YELLOW    
    #define DAEMON_FOREGROUND_PURPLE    
    #define DAEMON_FOREGROUND_CYAN      
    #define DAEMON_FOREGROUND_WHITE     

    #define DAEMON_BACKGROUND_GREY      
    #define DAEMON_BACKGROUND_RED       
    #define DAEMON_BACKGROUND_GREEN     
    #define DAEMON_BACKGROUND_BLUE      
    #define DAEMON_BACKGROUND_YELLOW    
    #define DAEMON_BACKGROUND_PURPLE    
    #define DAEMON_BACKGROUND_CYAN      
    #define DAEMON_BACKGROUND_WHITE     
#endif

USING_DAEMON_NAMESPACE

String ConsoleFormatter::ComputeLabel(LogRecord const& in_record) const noexcept
{
    switch (in_record.level)
    {
        #ifdef DAEMON_OS_WINDOWS

        case ELogLevel::NotSet:
            SetConsoleTextAttribute(m_handle, DAEMON_FOREGROUND_WHITE);
            break;

        case ELogLevel::Debug:
            SetConsoleTextAttribute(m_handle, DAEMON_FOREGROUND_GREY);
            break;

        case ELogLevel::Info:
            SetConsoleTextAttribute(m_handle, DAEMON_FOREGROUND_GREEN);
            break;

        case ELogLevel::Warning:
            SetConsoleTextAttribute(m_handle, DAEMON_FOREGROUND_YELLOW);
            break;

        case ELogLevel::Error:
            SetConsoleTextAttribute(m_handle, DAEMON_FOREGROUND_RED);
            break;

        case ELogLevel::Fatal:
            SetConsoleTextAttribute(m_handle, DAEMON_FOREGROUND_WHITE | DAEMON_BACKGROUND_RED);
            break;

        #else

        case ELogLevel::NotSet:     return "" + LogFormatter::ComputeLabel(in_record);
        case ELogLevel::Debug:      return "" + LogFormatter::ComputeLabel(in_record);
        case ELogLevel::Info:       return "" + LogFormatter::ComputeLabel(in_record);
        case ELogLevel::Warning:    return "" + LogFormatter::ComputeLabel(in_record);
        case ELogLevel::Error:      return "" + LogFormatter::ComputeLabel(in_record);
        case ELogLevel::Fatal:      return "" + LogFormatter::ComputeLabel(in_record);

        #endif
    }

    return LogFormatter::ComputeLabel(in_record);
}

#ifdef DAEMON_OS_WINDOWS

ConsoleFormatter::ConsoleFormatter() : LogFormatter(),
    m_handle { GetStdHandle(STD_OUTPUT_HANDLE) }
{
    
}

#else

ConsoleFormatter::ConsoleFormatter() : LogFormatter()
{

}

#endif

String ConsoleFormatter::Format(LogRecord const& in_record) const noexcept
{
    return LogFormatter::Format(in_record);
}

String ConsoleFormatter::FormatTime(LogRecord const& in_record) const noexcept
{
    return LogFormatter::FormatTime(in_record);
}

String ConsoleFormatter::FormatException(LogRecord const& in_record) const noexcept
{
    return LogFormatter::FormatException(in_record);
}

String ConsoleFormatter::FormatStack(LogRecord const& in_record) const noexcept
{
    return LogFormatter::FormatStack(in_record);
}