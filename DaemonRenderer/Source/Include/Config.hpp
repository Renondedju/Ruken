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

#define DAEMON_COPYRIGHT_STR "Copyright (c) 2019-2020 Basile Combet, Philippe Yi"
#define DAEMON_LICENSE_STR   "MIT License"
#define DAEMON_PROJECT_NAME  "Daemon Renderer"
#define DAEMON_URL           "https://github.com/Renondedju/Daemon"

// ------------------------------
//            Global Settings

#define DAEMON_CONFIG_LEVEL_DEBUG 0
#define DAEMON_CONFIG_LEVEL_RELEASE 1

#ifndef NDEBUG
    #define DAEMON_CONFIG_LEVEL DAEMON_CONFIG_LEVEL_DEBUG
    #define DAEMON_CONFIG_DEBUG
    #define DAEMON_CONFIG_STR "Debug"
#else
    #define DAEMON_CONFIG_LEVEL DAEMON_CONFIG_LEVEL_RELEASE
    #define DAEMON_CONFIG_RELEASE
    #define DAEMON_CONFIG_STR "Release"
#endif

#define DAEMON_DEBUG            if constexpr(DAEMON_CONFIG_LEVEL == DAEMON_CONFIG_LEVEL_DEBUG)
#define DAEMON_RELEASE          if constexpr(DAEMON_CONFIG_LEVEL == DAEMON_CONFIG_LEVEL_RELEASE)
#define DAEMON_DEBUG_RELEASE    if constexpr(DAEMON_CONFIG_LEVEL == DAEMON_CONFIG_LEVEL_DEBUG || DAEMON_CONFIG_LEVEL == DAEMON_CONFIG_LEVEL_RELEASE)

#if !defined(no_multithread) && defined(__STDCPP_THREADS__)
    #define DAEMON_MULTITHREAD_ENABLED
    #define DAEMON_MULTITHREAD_STATUS_STR "Enabled"
#else
    #define DAEMON_MULTITHREAD_DISABLED
    #define DAEMON_MULTITHREAD_STATUS_STR "Disabled"
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define DAEMON_OS_WINDOWS
    #define DAEMON_OS_STR "Windows"
#elif defined(__linux__)
    #define DAEMON_OS_LINUX
    #define DAEMON_OS_STR "Linux"
#elif defined(__unix__)
    #define DAEMON_OS_UNIX
    #define DAEMON_OS_STR "Unix"
#elif defined(_POSIX_VERSION)
    #define DAEMON_OS_POSIX
    #define DAEMON_OS_STR "Posix"
#elif defined(__APPLE__) || defined(__MACH__)
    #define DAEMON_OS_APPLE
    #define DAEMON_OS_STR "Apple"

    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
        #define DAEMON_OS_TARGET_APPLE_IPHONE_SIMULATOR
        #define DAEMON_OS_TARGET_STR "Iphone simulator"
    #elif TARGET_OS_IPHONE
        #define DAEMON_OS_TARGET_APPLE_IPHONE
        #define DAEMON_OS_TARGET_STR "Iphone"
    #else
        #define DAEMON_OS_TARGET_APPLE_OSX
        #define DAEMON_OS_TARGET_STR "OSX"
    #endif
#elif defined(__ANDROID__)
    #define DAEMON_OS_ANDROID
    #define DAEMON_OS_STR "Android"
#else
    #define DAEMON_OS_UNKNOWN
    #define DAEMON_OS_STR "Unknown"
#endif

#if defined(__x86_64__) || defined(__ppc64__) || defined(_WIN64)
    #define DAEMON_PLATFORM_X64
    #define DAEMON_PLATFORM_STR "x64"
#else
    #define DAEMON_PLATFORM_X86
    #define DAEMON_PLATFORM_STR "x86"
#endif

#if    defined(_MSC_VER)
    #define DAEMON_COMPILER_MSVC
    #define DAEMON_COMPILER_STR "msvc"
#elif defined(__ICL) || defined(__ICC) || defined(__INTEL_COMPILER)
    #define DAEMON_COMPILER_INTEL
    #define DAEMON_COMPILER_STR "intel"
#elif defined(__clang__)
    #define DAEMON_COMPILER_CLANG
    #define DAEMON_COMPILER_GCC // GCC-Compatible
    #define DAEMON_COMPILER_STR "clang"
#elif defined(__ghs__) // must appear _before_ __GNUC__ || __GNUG__
    #define DAEMON_COMPILER_GHS
    #define DAEMON_COMPILER_STR "ghs"
#elif defined(__GNUC__) || defined(__GNUG__)
    #define DAEMON_COMPILER_GCC
    #define DAEMON_COMPILER_STR "gcc"
#else
    #define DAEMON_COMPILER_UNKNOWN
    #define DAEMON_COMPILER_STR "unknown"
#endif

// ------------------------------
//            Namespace

#define DAEMON_NAMESPACE daemon
#define USING_DAEMON_NAMESPACE using namespace DAEMON_NAMESPACE;
#define BEGIN_DAEMON_NAMESPACE namespace DAEMON_NAMESPACE {
#define END_DAEMON_NAMESPACE }

// This avoids msvc error C2871 (or equivalent) to be triggered
namespace DAEMON_NAMESPACE {}

// ------------------------------
//            Threading

#if defined(DAEMON_CONFIG_DEBUG) || defined(DAEMON_CONFIG_RELEASE)
    #define DAEMON_THREADING_ENABLE_THREAD_LABELS
#else
    #define DAEMON_THREADING_DISABLE_THREAD_LABELS
#endif

// ------------------------------
//       Resource management

#if defined(DAEMON_CONFIG_DEBUG) || defined(DAEMON_CONFIG_RELEASE)
    #define DAEMON_RESOURCE_MANIFEST_STORE_IDENTIFIER
#endif

// ------------------------------
//              ECS

// Sets the maximum number of components allowed by the ECS, keep this number
// as low as possible. Must be a power of 2 with a minimum of 8.
#define DAEMON_MAX_ECS_COMPONENTS 64