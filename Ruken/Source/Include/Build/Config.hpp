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

// ------------------------------
//            Global Settings

#define RUKEN_CONFIG_LEVEL_DEBUG 0
#define RUKEN_CONFIG_LEVEL_RELEASE 1

#ifndef NDEBUG
    #define RUKEN_CONFIG_LEVEL RUKEN_CONFIG_LEVEL_DEBUG
    #define RUKEN_CONFIG_DEBUG
    #define RUKEN_CONFIG_STR "Debug"
#else
    #define RUKEN_CONFIG_LEVEL RUKEN_CONFIG_LEVEL_RELEASE
    #define RUKEN_CONFIG_RELEASE
    #define RUKEN_CONFIG_STR "Release"
#endif

#define RUKEN_DEBUG   if constexpr(RUKEN_CONFIG_LEVEL == RUKEN_CONFIG_LEVEL_DEBUG)
#define RUKEN_RELEASE if constexpr(RUKEN_CONFIG_LEVEL == RUKEN_CONFIG_LEVEL_RELEASE)

#if !defined(no_multithread) && defined(__STDCPP_THREADS__)
    #define RUKEN_MULTITHREAD_ENABLED
    #define RUKEN_MULTITHREAD_STATUS_STR "Enabled"
#else
    #define RUKEN_MULTITHREAD_DISABLED
    #define RUKEN_MULTITHREAD_STATUS_STR "Disabled"
#endif

// ------------------------------
//            Threading

#if defined(RUKEN_CONFIG_DEBUG)
    #define RUKEN_THREADING_ENABLE_THREAD_LABELS
#else
    #define RUKEN_THREADING_DISABLE_THREAD_LABELS
#endif

// ------------------------------
//       Resource management

#if defined(RUKEN_CONFIG_DEBUG)
    #define RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER
#endif

// ------------------------------
//              ECS

// Sets the maximum number of components allowed by the ECS, keep this number
// as low as possible. Must be a power of 2 with a minimum of 8.
#define RUKEN_MAX_ECS_COMPONENTS 64