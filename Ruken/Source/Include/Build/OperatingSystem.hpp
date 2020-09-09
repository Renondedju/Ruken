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

#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define RUKEN_OS_WINDOWS
    #define RUKEN_OS_STR "Windows"
#elif defined(__linux__)
    #define RUKEN_OS_LINUX
    #define RUKEN_OS_STR "Linux"
#elif defined(__unix__)
    #define RUKEN_OS_UNIX
    #define RUKEN_OS_STR "Unix"
#elif defined(_POSIX_VERSION)
    #define RUKEN_OS_POSIX
    #define RUKEN_OS_STR "Posix"
#elif defined(__APPLE__) || defined(__MACH__)
    #define RUKEN_OS_APPLE
    #define RUKEN_OS_STR "Apple"

    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
        #define RUKEN_OS_TARGET_APPLE_IPHONE_SIMULATOR
        #define RUKEN_OS_TARGET_STR "Iphone simulator"
    #elif TARGET_OS_IPHONE
        #define RUKEN_OS_TARGET_APPLE_IPHONE
        #define RUKEN_OS_TARGET_STR "Iphone"
    #else
        #define RUKEN_OS_TARGET_APPLE_OSX
        #define RUKEN_OS_TARGET_STR "OSX"
    #endif
#elif defined(__ANDROID__)
    #define RUKEN_OS_ANDROID
    #define RUKEN_OS_STR "Android"
#else
    #define RUKEN_OS_UNKNOWN
    #define RUKEN_OS_STR "Unknown"
#endif