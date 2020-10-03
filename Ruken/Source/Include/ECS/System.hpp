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
#include <vector>

#include "Build/Namespace.hpp"

#include "ECS/Group.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/SystemBase.hpp"

#include "Meta/PassConst.hpp"
#include "Meta/TupleApply.hpp"
#include "Meta/TupleSubset.hpp"
#include "Meta/TupleHasType.hpp"

#include "Types/FundamentalTypes.hpp"

#include "ECS/Meta/ComponentHelper.hpp"
#include "ECS/Safety/ComponentType.hpp"
#include "ECS/Safety/SparseComponentType.hpp"
#include "ECS/Safety/ExclusiveComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

class EntityAdmin;

/**
 * \brief Systems transform data. They implement the logic that modifies the components.
 *
 * It is important to notice that for lots of actions, you don�t care about the specific type of an entity;
 * what you care about is specific properties of these entities.
 * E.g. for rendering all you need is a mesh and a transform matrix; you don�t care if the entity is a player or a tree.
 *
 * \tparam TComponents Required components for the system to operate
 * \note Passing a component type with a const modifier will notify the ECS that this component is readonly for this system
 *       Meaning that the ECS can enforce constant access to every field of the component via the ComponentViews.
 *       This will also notify the ECS update manager that a potential optimization can be done when updating systems,
 *       using multithreading, reducing frame times further along.
 */
template <ComponentType... TComponents>
class System : public SystemBase
{
    protected:

        #pragma region Usings

        // Tuples containing a specific type of component
        using SparseComponents    = typename TupleSubset<IsSparseComponent   , TComponents...>::Type;
        using ExclusiveComponents = typename TupleSubset<IsExclusiveComponent, TComponents...>::Type;

        // For now non exclusive components only consists of SparseComponents
        using IterativeComponents      = decltype(std::tuple_cat(std::declval<SparseComponents>()));
        using IterativeComponentsGroup = typename TupleApply<Group, IterativeComponents>::Type;

        /**
         * \brief Returns the access type of the exclusive component based on the input component constness (system template) and on the actual component passed (TComponent)
         *        If one of the 2 is const, the return type will be const
         * \tparam TComponent Component to look for
         */
        template <ExclusiveComponentType TComponent> requires ComponentHelper<ExclusiveComponents>::template ComponentExists<TComponent>::value
        using ExclusiveComponentAccess = PassConst<TComponent, std::tuple_element_t<ComponentHelper<ExclusiveComponents>::template ComponentIndex<TComponent>::value, ExclusiveComponents>>;

        #pragma endregion

        #pragma region Members

        // This vector stores every group we wish to read/write
        // You really should not try to edit the vector itself yourself.
        // Instead, use it for iteration purposes only !
        std::vector<IterativeComponentsGroup> m_groups {};
        EntityAdmin&                          m_admin;

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

        /**
         * \brief Returns a reference onto the requested exclusive component
         * \note If this is the first access to the designated exclusive component, this method will allocate the component
         * \tparam TExclusiveComponent exclusive component type
         * \return Exclusive component reference
         */
        template <ExclusiveComponentType TExclusiveComponent>
        ExclusiveComponentAccess<TExclusiveComponent>& GetExclusiveComponent() noexcept;

        #pragma endregion

        #pragma region Operators

        System& operator=(System const& in_copy) = delete;
        System& operator=(System&&      in_move) = delete;

        #pragma endregion
};

#include "ECS/System.inl"

END_RUKEN_NAMESPACE