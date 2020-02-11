/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include <iostream>

#include "Config.hpp"

BEGIN_DAEMON_NAMESPACE

#define DAEMON_STATIC_ASSERT(in_expr, in_message) static_assert(in_expr, in_message)
#define DAEMON_ASSERT_MESSAGE(in_expr, in_message) internal::Assert(#in_expr, in_expr, __FILE__, __LINE__, in_message)

namespace internal
{
    inline void Assert(const char* in_expr_str, bool const in_expr, const char* in_file, int const in_line, const char* in_msg)
    {
        if (!in_expr)
        {
            std::cerr << "Assert failed: " << in_msg      << '\n'
                      << "Expected: "      << in_expr_str << '\n'
                      << "Source: "        << in_file     << ", line " << in_line << '\n';
            abort();
        }
    }
}

END_DAEMON_NAMESPACE