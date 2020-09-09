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
#include "ECS/EntityID.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Base class of the Component class, this is simply used to store components as they are templated
 */
class ComponentBase
{
    public:

        #pragma region Constructors

        ComponentBase()                             = default;
        ComponentBase(ComponentBase const& in_copy) = default;
        ComponentBase(ComponentBase&&      in_move) = default;
        virtual ~ComponentBase()                    = default;

        #pragma endregion

        #pragma region Methods

        virtual RkVoid CreateItemAt(RkSize in_index) noexcept;

        #pragma endregion

        #pragma region Operators

        ComponentBase& operator=(ComponentBase const& in_copy) = default;
        ComponentBase& operator=(ComponentBase&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE
