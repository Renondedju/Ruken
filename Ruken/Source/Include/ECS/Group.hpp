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

#include <tuple>

#include "Build/Namespace.hpp"

#include "Meta/CopyConst.hpp"
#include "Meta/TupleIndex.hpp"

#include "ECS/Safety/ViewType.hpp"
#include "ECS/Safety/AnyComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief Systems do not work on archetypes immediately.
 *        It would be tedious to declare the exhaustive list of archetypes to work with.
 *        Instead, systems describe their requirements as a query;
 *        each group storing a reference onto the matching components (from possibly multiple archetypes),
 *        thus allowing component access under a unified view.
 *        
 * \tparam TComponents Component types to keep a reference onto
 */
template <AnyComponentType... TComponents>
class Group
{
    // If a component has been passed as const, those helpers will ensure that the
    // GetComponent method will return a constant reference onto the returned component
    // thus, enforcing the usage of readonly views to access any field of the component

    template <AnyComponentType TComponent> using ComponentIndex     = TupleIndex<std::remove_const_t<TComponent>, std::tuple<std::remove_const_t<TComponents>...>>;
    template <AnyComponentType TComponent> using ComponentReference = CopyConst<std::tuple_element_t<ComponentIndex<TComponent>::value, std::tuple<TComponents...>>, TComponent>&;

    private:

        #pragma region Members

        Archetype&                  m_archetype;
        std::tuple<TComponents&...> m_components {};

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns a view containing all the requested fields
         * \tparam TFields Requested fields
         * \return View containing all the requested fields
         */
        template <ComponentFieldType... TFields>
        [[nodiscard]] ComponentView<TFields...> GetViewInternal(Tag<ComponentView<TFields...>>) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Referencing group constructor
         * \param in_archetype Referenced archetype of the group
         * \param in_components Referenced components of the group
         */
        Group(Archetype& in_archetype, TComponents&... in_components) noexcept;

        Group(Group const& in_copy) = default;
        Group(Group&&      in_move) = default;
        ~Group()                    = default;

        #pragma endregion

        /**
         * \brief Returns a view containing all the requested fields
         * \tparam TView Requested view
         * \return View containing all the requested fields
         */
        template <ViewType TView>
        [[nodiscard]] TView GetView() noexcept;

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
        template <AnyComponentType TComponent>
        ComponentReference<TComponent> GetComponent() noexcept
        {
            // BUG: This method has to be inlined because of a msvc bug (fix your shit Microsoft please)
            return std::get<TupleIndex<std::remove_const_t<TComponent>, std::tuple<std::remove_const_t<TComponents>...>>::value>(m_components);
        }

        #pragma region Operators

        Group& operator=(Group const& in_copy) = delete;
        Group& operator=(Group&&      in_move) = delete;

        #pragma endregion
};

#include "ECS/Group.inl"

END_RUKEN_NAMESPACE
