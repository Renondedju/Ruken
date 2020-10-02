/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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