
#pragma once

#include <iostream>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

#define RUKEN_STATIC_ASSERT(...) static_assert(__VA_ARGS__)
#define RUKEN_ASSERT_MESSAGE(in_expr, in_message) internal::Assert(#in_expr, in_expr, __FILE__, __LINE__, in_message)

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

END_RUKEN_NAMESPACE