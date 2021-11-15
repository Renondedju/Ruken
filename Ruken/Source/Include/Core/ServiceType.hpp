
#pragma once

#include <type_traits>

#include "Core/Service.hpp"
#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TType>
concept ServiceType = std::is_base_of<Service<TType>, TType>::value;

END_RUKEN_NAMESPACE