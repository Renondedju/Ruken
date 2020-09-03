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

#define RUKEN_COPYRIGHT_STR "Copyright (c) 2019-2020 Basile Combet, Philippe Yi"
#define RUKEN_LICENSE_STR   "MIT License"
#define RUKEN_PROJECT_NAME  "Ruken"
#define RUKEN_URL           "https://github.com/Renondedju/Ruken"

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

#if defined(__x86_64__) || defined(__ppc64__) || defined(_WIN64)
    #define RUKEN_PLATFORM_X64
    #define RUKEN_PLATFORM_STR "x64"
#else
    #define RUKEN_PLATFORM_X86
    #define RUKEN_PLATFORM_STR "x86"
#endif

#if    defined(_MSC_VER)
    #define RUKEN_COMPILER_MSVC
    #define RUKEN_COMPILER_STR "msvc"
#elif defined(__ICL) || defined(__ICC) || defined(__INTEL_COMPILER)
    #define RUKEN_COMPILER_INTEL
    #define RUKEN_COMPILER_STR "intel"
#elif defined(__clang__)
    #define RUKEN_COMPILER_CLANG
    #define RUKEN_COMPILER_GCC // GCC-Compatible
    #define RUKEN_COMPILER_STR "clang"
#elif defined(__ghs__) // must appear _before_ __GNUC__ || __GNUG__
    #define RUKEN_COMPILER_GHS
    #define RUKEN_COMPILER_STR "ghs"
#elif defined(__GNUC__) || defined(__GNUG__)
    #define RUKEN_COMPILER_GCC
    #define RUKEN_COMPILER_STR "gcc"
#else
    #define RUKEN_COMPILER_UNKNOWN
    #define RUKEN_COMPILER_STR "unknown"
#endif

// ------------------------------
//            Namespace

#define RUKEN_NAMESPACE ruken
#define USING_RUKEN_NAMESPACE using namespace RUKEN_NAMESPACE;
#define BEGIN_RUKEN_NAMESPACE namespace RUKEN_NAMESPACE {
#define END_RUKEN_NAMESPACE }

// This avoids msvc error C2871 (or equivalent) to be triggered
namespace RUKEN_NAMESPACE {}

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