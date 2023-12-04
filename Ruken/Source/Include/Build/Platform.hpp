#pragma once

#if defined(__x86_64__) || defined(__ppc64__) || defined(_WIN64)
    #define RUKEN_PLATFORM_X64
    #define RUKEN_PLATFORM_STR "x64"
#else
    #define RUKEN_PLATFORM_X86
    #define RUKEN_PLATFORM_STR "x86"
#endif