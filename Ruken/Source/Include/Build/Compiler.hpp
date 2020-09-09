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