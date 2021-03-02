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

#include "Build/Namespace.hpp"
#include "Meta/IsBaseOfTemplate.hpp"
#include "ECS/Safety/ComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

template <EEventName TEventName, ComponentType... TComponents>
class EventHandler;

template <template <EEventName, typename...> class TBase, typename TDerived>
struct IsBaseOfEventHandlerImpl
{
    template<EEventName TEventName, typename... TTypes>
    static constexpr std::true_type  Test(const TBase<TEventName, TTypes...>*) { return {}; }
    static constexpr std::false_type Test(...)                                 { return {}; }

    using Type = decltype(Test(std::declval<TDerived*>()));
};

template<typename TType>
using IsEventHandler = typename IsBaseOfEventHandlerImpl<EventHandler, TType>::Type;

template <typename TType>
concept EventHandlerType = IsEventHandler<TType>::value;

END_RUKEN_NAMESPACE