#pragma once

#include "Build/BuildInfo.hpp"

// stdint.h is used instead of cstdint on purpose, please don't modify this.
#ifdef RUKEN_COMPILER_MSVC
    #include <stdint.h>

    using RkInt8  = int8_t;
    using RkInt16 = int16_t;
    using RkInt32 = int32_t;
    using RkInt64 = int64_t;

    using RkUint8  = uint8_t;
    using RkUint16 = uint16_t;
    using RkUint32 = uint32_t;
    using RkUint64 = uint64_t;

    using RkSize = size_t;
#else
    #include <cstdint>

    using RkInt8  = std::int8_t;
    using RkInt16 = std::int16_t;
    using RkInt32 = std::int32_t;
    using RkInt64 = std::int64_t;

    using RkUint8  = std::uint8_t;
    using RkUint16 = std::uint16_t;
    using RkUint32 = std::uint32_t;
    using RkUint64 = std::uint64_t;

    using RkSize = std::size_t;
#endif

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

// Booleans
using RkBool = bool;

//Characters
using RkChar   = char;
using RkSchar  = signed char;
using RkUchar  = unsigned char;
using RkWchar  = wchar_t;
using RkChar8  = signed char;
using RkChar16 = char16_t;
using RkChar32 = char32_t;

// Integers
using RkInt  = int;
using RkByte = char;

// Floating points
using RkFloat   = float;
using RkDouble  = double;
using RkLdouble = long double;
using RkULLInt = unsigned long long int;

// Others
using RkVoid = void;

END_RUKEN_NAMESPACE