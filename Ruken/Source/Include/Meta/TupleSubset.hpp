
#pragma once

#include <tuple>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template<template <typename> typename TConcept, typename TTuple, typename TValidatedTuple>
struct TupleSubsetImplementation;

template<template <typename TType> typename TConcept, typename TTestedArg, typename... TArgs, typename... TValidatedArgs>
struct TupleSubsetImplementation<TConcept, std::tuple<TTestedArg, TArgs...>, std::tuple<TValidatedArgs...>>:
    TupleSubsetImplementation<TConcept, std::tuple<TArgs...>, std::conditional_t<TConcept<TTestedArg>::value, std::tuple<TTestedArg, TValidatedArgs...>, std::tuple<TValidatedArgs...>>>
{ };

template<template <typename> typename TConcept, typename... TValidatedArgs>
struct TupleSubsetImplementation<TConcept, std::tuple<>, std::tuple<TValidatedArgs...>>
{
    using ValidatedTuple = std::tuple<TValidatedArgs...>;
};

template<template <typename> typename TConcept, typename... TArgs>
struct TupleSubset
{
    using Type = typename TupleSubsetImplementation<TConcept, std::tuple<TArgs...>, std::tuple<>>::ValidatedTuple;
};

END_RUKEN_NAMESPACE