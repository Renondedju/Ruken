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

#include <type_traits>

#include "Build/Namespace.hpp"

#include "Meta/PassConst.hpp"
#include "Meta/CopyConst.hpp"

#include "ECS/Range.hpp"
#include "ECS/FieldHelper.hpp"
#include "ECS/Safety/FieldType.hpp"

#include "Containers/LinkedChunkListNode.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Allows to fetch only the required fields in a component, saving on data bus bandwidth and cache
 * \note All instances of this class are generated via the item type of each component
 * \tparam TPack Index pack enumerating the index of the members to fetch
 * \tparam TFields Member types to create a reference onto
 *                 Fields can be constant, meaning that they will be forced to be readonly on every fetch
 */
template <typename TPack, FieldType... TFields>
class ComponentView;

template <template <RkSize...> class TPack, RkSize... TIndices, FieldType... TFields>
class ComponentView<TPack<TIndices...>, TFields...>
{
    using Helper = FieldHelper<TFields...>;

    public:

        #pragma region Usings

        using IsReadonly         = typename Helper::Readonly;
        using FieldIndexSequence = std::index_sequence<TIndices...>;

        template <FieldType TField> using FieldChunk    = LinkedChunkListNode<typename TField::Type>;
        template <FieldType TField> using ReferencePair = std::pair<RkSize, FieldChunk<TField>*>;

        /**
         * \brief Returns the access type of the field
         * \tparam TField Field to look for
         */
        template <FieldType TField> requires Helper::template FieldExists<TField>::value
        using FieldAccess = CopyConst<PassConst<TField, std::tuple_element_t<Helper::template FieldIndex<TField>::value, std::tuple<TFields...>>>, typename TField::Type>;

        #pragma endregion 

    private:

        #pragma region Members

        // Stores a tuple of
        // - An index containing the current node index of field container
        // - A pointer to the actual field node container 
        std::tuple<ReferencePair<TFields>...> m_fields_references;

        // Reference to the next empty range of the archetype
        // This is used to skip de-allocated entities when iterating
        std::list<Range>::const_iterator m_next_empty_range;

        // Actual owning archetype of the data we want to iterate
        Archetype const& m_component_archetype;

        // Current entity index we are referencing (local entity identifier)
        RkSize m_index {0ULL};

        #pragma endregion 

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_archetype Iterated component archetype. This is used to automatically skip de-allocated entities 
         * \param in_fields Fields to iterate on
         */
        ComponentView(Archetype const& in_archetype, FieldChunk<TFields>*... in_fields) noexcept;

        ComponentView(ComponentView const& in_copy) = default;
        ComponentView(ComponentView&&      in_move) = default;
        ~ComponentView()                            = default;

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Updates the view to reference the next entity found, if the view found nothing, false is returned
         * \return True if the next entity has been found, false otherwise
         */
        [[nodiscard]] RkBool FindNextEntity() noexcept;

        /**
         * \brief Fetches a field of the currently referenced entity
         * \tparam TField Field type, must be contained in the view
         * \return Reference to the entity field
         */
        template <FieldType TField>
        [[nodiscard]] FieldAccess<TField>& Fetch() const noexcept;

        #pragma endregion 

        #pragma region Operators

        ComponentView& operator=(ComponentView const& in_copy) = default;
        ComponentView& operator=(ComponentView&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ComponentView.inl"

END_RUKEN_NAMESPACE
