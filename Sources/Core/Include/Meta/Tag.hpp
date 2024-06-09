
#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Allows the passage of multiple types as arguments for template deduction without any allocation 
 * \tparam TTypes Types
 */
template <typename... TTypes>
class Tag
{ };

END_RUKEN_NAMESPACE