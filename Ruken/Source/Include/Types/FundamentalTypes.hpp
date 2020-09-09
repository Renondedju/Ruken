/**
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

// stdint.h is used instead of cstdint on purpose, please don't modify this.
#include <stdint.h>

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

using RkInt = int;

using RkInt8  = int8_t;
using RkInt16 = int16_t;
using RkInt32 = int32_t;
using RkInt64 = int64_t;

using RkUint8  = uint8_t;
using RkUint16 = uint16_t;
using RkUint32 = uint32_t;
using RkUint64 = uint64_t;

using RkSize = size_t;
using RkByte = RkUint8;

// Floating points

using RkFloat   = float;
using RkDouble  = double;
using RkLdouble = long double;

// Others

using RkVoid = void;

END_RUKEN_NAMESPACE