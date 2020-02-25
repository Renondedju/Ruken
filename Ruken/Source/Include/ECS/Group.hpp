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
#include "Containers/Tuple.hpp"

BEGIN_DAEMON_NAMESPACE

class Archetype;

template <typename... TComponents>
class Group
{
    private:

        #pragma region Members

        Archetype&             m_archetype;
        Tuple<TComponents&...> m_components;

        #pragma endregion

    public:

        #pragma region Constructors

        Group(Archetype& in_archetype, TComponents&... in_components) noexcept;

        Group(Group const& in_copy) = default;
        Group(Group&&      in_move) = default;
        ~Group()                    = default;

        #pragma endregion

        /**
         * \brief Returns the referenced archetype
         * \return Referenced archetype
         */
        Archetype& GetReferencedArchetype() const noexcept;

        /**
         * \brief Returns a component with the given type
         * \tparam TComponent Component type to return
         * \return Component reference
         */
        template<typename TComponent>
        TComponent& GetComponent() noexcept;

        #pragma region Operators

        Group& operator=(Group const& in_copy) = default;
        Group& operator=(Group&&      in_move) = default;

        #pragma endregion
};

#include "ECS/Group.inl"

END_DAEMON_NAMESPACE