#pragma once

#include "Core/Build/Namespace.hpp"

#include <tuple>

BEGIN_RUKEN_NAMESPACE

template <typename TTuple>
struct TupleRemoveCvRef;

template <typename... TTypes>
struct TupleRemoveCvRef<std::tuple<TTypes...>>
{
	using Type = std::tuple<std::remove_cvref_t<TTypes>...>;
};

END_RUKEN_NAMESPACE