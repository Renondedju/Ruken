#pragma once

#if   defined(_MSC_VER)
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