/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Meta/IsInstance.hpp"
#include "Meta/IsBaseOfTemplate.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Defines a field (or variable) for a component.
 *        In order to define a field you HAVE to inherit from this class.
 *        The name of the inheriting class will then be the way to refer to your field.
 *        Since this name might not be unique, it is recommended to put these classes in unique namespaces for each component to avoid interferences.
 * \warning We strongly discourage the reuse of the field across multiple components even if this is possible.
 * \warning A field can only be used once per component, doing otherwise will result in undefined behaviors
 * \tparam TDataType Type of the field
 */
template <typename TDataType>
struct ComponentField
{
    using Type = TDataType;
};

/**
 * \brief Checks if the passed type is a valid field
 *        The passed type must:
 *        - Be a direct inheritance from the ComponentField class
 *        - Not be a direct instance of the ComponentField class
 *        - Not be volatile
 * \tparam TType Type to check
 */
template <typename TType>
concept FieldType = IsBaseOfTemplate<ComponentField, TType>::value && !IsInstance<TType, ComponentField>::value && !std::is_volatile_v<TType>;

END_RUKEN_NAMESPACE