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
#include <utility>

// MinExceptZero
#include <Common/Numerics.hpp>

#include "Build/Namespace.hpp"

#include "Meta/IndexPack.hpp"
#include "Meta/TupleIndex.hpp"

#include "ECS/ComponentView.hpp"
#include "ECS/ComponentField.hpp"

#include "Types/FundamentalTypes.hpp"
#include "Containers/LinkedChunkList.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief Describes a component layout as well as helpers to interact with that memory layout
 * \tparam TFields Fields of the component, must be of type ComponentField
 *
 * \see ComponentItem, ComponentView, ComponentField, Component
 */
template <FieldType... TFields>
class ComponentLayout
{
    public:

        #pragma region Usings

        using ContainerType = std::tuple<LinkedChunkList<typename TFields::Type>...>;

        /**
         * \brief Returns the index of a member using the member class
         * \tparam TField Field class
         */
        template <FieldType TField>
        using FieldIndex = TupleIndex<std::remove_const_t<TField>, std::tuple<TFields...>>;

        // View constructors
        template <FieldType... TSelectedFields>
        using MakeView         = ComponentView<IndexPack<FieldIndex<TSelectedFields>::value...>, TSelectedFields...>;
        using FullView         = MakeView<TFields...>;
        using FullReadonlyView = MakeView<TFields const...>;

        #pragma endregion 

    private:

        #pragma region Methods

        /**
         * \brief GetView helper
         */
        template <ViewType TView, RkSize... TIds>
        static TView GetViewHelper(ContainerType& in_container, Archetype const& in_owning_archetype, std::index_sequence<TIds...>) noexcept;

        /**
         * \brief EnsureStorageSpace
         */
        template <RkSize... TIds>
        static RkSize EnsureStorageSpaceHelper(ContainerType& in_container, RkSize in_size, std::index_sequence<TIds...>) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        ComponentLayout()                               = default;
        ComponentLayout(ComponentLayout const& in_copy) = default;
        ComponentLayout(ComponentLayout&&      in_move) = default;
        ~ComponentLayout()                              = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the requested view of a component storage
         * \note The view will point onto the head of each field
         * \tparam TView Requested view type
         * \param in_container component container
         * \param in_owning_archetype Owning archetype
         * \return Requested view instance
         */
        template <ViewType TView>
        static TView GetView(ContainerType& in_container, Archetype const& in_owning_archetype) noexcept;

        /**
         * \brief Ensures that the passed container will have the storage space required for a given amount of entities
         *        If the size of the container is already sufficient, this method will do nothing,
         *        otherwise memory will be allocated to fill the missing required space
         *
         * \param in_container Container to ensure/allocate memory into
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        static RkSize EnsureStorageSpace(ContainerType& in_container, RkSize in_size) noexcept;

        #pragma endregion 

        #pragma region Operators

        ComponentLayout& operator=(ComponentLayout const& in_copy) = default;
        ComponentLayout& operator=(ComponentLayout&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ComponentLayout.inl"

END_RUKEN_NAMESPACE