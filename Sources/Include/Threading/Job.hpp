
#pragma once

#include <functional>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

using Job = std::function<RkVoid()>;

END_RUKEN_NAMESPACE