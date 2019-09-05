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

#include "ECS/ComponentID.hpp"

#include "Containers/UnorderedMap.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Base component class
 * \tparam TDerived Derived class
 */
template <typename TDerived>
class Component
{
    private:

        #pragma region Variables

        ComponentID m_id;

        // This is not the best container for this usage, ideally we would like to have
        // a cache friendly associative container. This does the work for now 
        static UnorderedMap<ComponentID, TDerived> m_storage;

        // Id cache, used to generate new ids
        static ComponentID m_id_cache;

        #pragma endregion

    public:

        #pragma region Constructors

        Component()                         noexcept;
        Component(Component const& in_copy) noexcept = default;
        Component(Component&&      in_move) noexcept = default;
        ~Component()                        noexcept;

        #pragma endregion

        #pragma region Operators

        Component& operator=(Component const& in_copy) noexcept = default;
        Component& operator=(Component&&      in_move) noexcept = default;

        #pragma endregion
};

#include "ECS/Component.inl"

END_DAEMON_NAMESPACE