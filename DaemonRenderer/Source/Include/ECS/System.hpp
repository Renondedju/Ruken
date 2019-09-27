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

#include "Types/FundamentalTypes.hpp"

#include "ECS/ComponentID.hpp"

#include "Containers/Vector.hpp"
#include "Containers/Tuple.hpp"
#include "Containers/Array.hpp"

BEGIN_DAEMON_NAMESPACE

template <typename... TRequiredComponents>
class __declspec(novtable) System
{
    protected:

        #pragma region Members

        // Array of all subscribed components
        Vector<Array<ComponentID, sizeof...(TRequiredComponents)>> m_subscribers;
        Tuple<TRequiredComponents& ...>                            m_components;

        #pragma endregion 

    public:

        #pragma region Constructors

        System()                      noexcept = default;
        System(System const& in_copy) noexcept = default;
        System(System&&	     in_move) noexcept = default;
        virtual ~System()             noexcept = default;

        #pragma endregion

        #pragma region Methods

        virtual void Update(DAEfloat in_delta_time) = 0;

        #pragma endregion

        #pragma region Operators

        System& operator=(System const& in_copy) noexcept = default;
        System& operator=(System&&	    in_move) noexcept = default;

        #pragma endregion
};


END_DAEMON_NAMESPACE