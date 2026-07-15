#pragma once

#include "Core/Build/Namespace.hpp"

#include <assert.h>

BEGIN_RUKEN_NAMESPACE

#define RUKEN_STATIC_ASSERT(...) static_assert(__VA_ARGS__)
#define RUKEN_ASSERT(in_expr, in_message) assert(((in_expr) && (in_message)))

END_RUKEN_NAMESPACE