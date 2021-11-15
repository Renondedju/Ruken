
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

// ------------------------------
//            Threading

#if !defined(no_multithread) && defined(__STDCPP_THREADS__) && !defined(RUKEN_REQUEST_SINGLE_THREADED_BUILD)
    #define RUKEN_MULTITHREAD_ENABLED
    #define RUKEN_MULTITHREAD_STATUS_STR "Enabled"
#else
    #define RUKEN_MULTITHREAD_DISABLED
    #define RUKEN_MULTITHREAD_STATUS_STR "Disabled"
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

// ------------------------------
//            Logging

#if !defined(RUKEN_REQUEST_SILENT_BUILD)
    #define RUKEN_LOGGING_ENABLED
    #define RUKEN_LOGGING_STATUS_STR "Enabled"
#else
    #define RUKEN_LOGGING_DISABLED
    #define RUKEN_LOGGING_STATUS_STR "Disabled"
#endif