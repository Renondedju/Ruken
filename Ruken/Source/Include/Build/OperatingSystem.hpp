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