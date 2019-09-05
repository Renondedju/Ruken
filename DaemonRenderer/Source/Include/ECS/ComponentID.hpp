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

#include "Config.hpp"

#include "Types/Operators/Arithmetic/Increment.hpp"
#include "Types/Operators/Comparison.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Types/NamedType.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Strong typing of a component ID
 */
class ComponentID : public NamedType<DAEsize, ComponentID>,
                    public Comparison<ComponentID>,
                    public Increment <ComponentID>
{
    using Parent = NamedType<DAEsize, ComponentID>;

    public:

        #pragma region Constructors

        ComponentID()                           noexcept = default;
        ComponentID(ComponentID const& in_copy) noexcept = default;
        ComponentID(ComponentID&&      in_move) noexcept = default;
        ~ComponentID()                          noexcept = default;

        using Parent::Parent;

        #pragma endregion

        #pragma region Operators

        ComponentID& operator=(ComponentID const& in_copy) noexcept = default;
        ComponentID& operator=(ComponentID&&      in_move) noexcept = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE