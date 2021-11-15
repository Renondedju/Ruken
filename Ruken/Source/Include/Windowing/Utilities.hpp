
#pragma once

#define GLFW_INCLUDE_NONE

#include <string>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

std::string GlfwErrorToString(RkInt32 in_error_code) noexcept;

END_RUKEN_NAMESPACE