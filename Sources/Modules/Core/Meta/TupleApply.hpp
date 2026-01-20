#pragma once

#include <tuple>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Applies all the tuple types to a host class
 * \tparam THost Host class
 * \tparam TTuple Original tuple
 */
template <template <typename ...> class THost, typename TTuple>
struct TupleApply;

template <template <typename ...> class THost, typename... TTypes>
struct TupleApply<THost, std::tuple<TTypes...>>
{
    using Type = THost<TTypes...>;
};

END_RUKEN_NAMESPACE