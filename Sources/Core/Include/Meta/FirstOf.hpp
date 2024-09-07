#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <                 typename... TTypes> struct FirstOf                    { using Type = void;   };
template <typename TFirst, typename... TTypes> struct FirstOf<TFirst, TTypes...> { using Type = TFirst; };

END_RUKEN_NAMESPACE
