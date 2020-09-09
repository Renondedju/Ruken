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

#include <vector>

#include "Config.hpp"

#include "ECS/Group.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/SystemBase.hpp"

BEGIN_RUKEN_NAMESPACE

class EntityAdmin;

/**
 * \brief Systems transform data. They implement the logic that modifies the components.
 *
 * It is important to notice that for lots of actions, you don’t care about the specific type of an entity;
 * what you care about is specific properties of these entities.
 * E.g. for rendering all you need is a mesh and a transform matrix; you don’t care if the entity is a player or a tree.
 *
 * \tparam TComponents Required components for the system to operate
 */
template <typename... TComponents>
class System : public SystemBase
{
    protected:

        #pragma region Members

        // This vector stores every group we wish to read/write
        // You really should not try to edit the vector itself yourself.
        // Instead, use it for iteration purposes only !
        std::vector<Group<TComponents...>> m_groups {};
        EntityAdmin&                       m_admin;

        #pragma endregion

    public:

        #pragma region Constructors

        System(EntityAdmin&  in_admin) noexcept;

        System(System const& in_copy) = default;
        System(System&&      in_move) = default;
        virtual ~System() override    = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Adds a component reference group to the system.
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        virtual RkVoid AddReferenceGroup(Archetype& in_archetype) noexcept override final;

        #pragma endregion

        #pragma region Operators

        System& operator=(System const& in_copy) = delete;
        System& operator=(System&&      in_move) = delete;

        #pragma endregion
};

#include "ECS/System.inl"

END_RUKEN_NAMESPACE