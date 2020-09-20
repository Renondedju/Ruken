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

#include "Build/Config.hpp"
#include "Build/Namespace.hpp"

#include "Meta/Meta.hpp"
#include "Meta/Assert.hpp"

#include "ECS/ComponentBase.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A tag component does not hold any data, its only purpose is to differentiate 2 entities from each other by "tagging" it.
 * \tparam TUniqueId Unique ID of the component.
 *                   Ideally this would be generated automatically at compile time but doing so in c++ is
 *                   really hard and not 100% reliable. Since this ID must be unique for each component, the best way to maintain it
 *                   is using an enum enumerating every component of the game. This way if a component is deleted, the ids of every other
 *                   component will be maintained automatically. This enum must use the default values in order to work. See examples for more info.
 */
template <RkSize TUniqueId>
class TagComponent final : public ComponentBase
{
    RUKEN_STATIC_ASSERT(TUniqueId < RUKEN_MAX_ECS_COMPONENTS, "Please increate the maximum amount of ECS components to run this program.");

    public:

        static constexpr RkSize id = TUniqueId;

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning_archetype Owning archetype
         */
        TagComponent(Archetype const& in_owning_archetype) noexcept;

        TagComponent(TagComponent const& in_copy) = default;
        TagComponent(TagComponent&&      in_move) = default;
        virtual ~TagComponent() override          = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \note Since a tag component does not contain any data, this method does nothing
         *       The returned value is also always UINTMAX_MAX to allow allocators (archetypes) to call this method as few times as possible
         *
         * \brief Ensures that the component has enough storage space for a given amount of entities
         *        If this is not the case, containers will be allocated
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the component layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        virtual RkSize EnsureStorageSpace(RkSize in_size) noexcept override;

        #pragma endregion 

        #pragma region Operators

        TagComponent& operator=(TagComponent const& in_copy) = default;
        TagComponent& operator=(TagComponent&&      in_move) = default;

        #pragma endregion
};

#include "ECS/TagComponent.inl"

/**
 * \brief Shorthand to declare a tag component named "<in_component_name>Component"
 * \param in_component_name Name of the component as defined in the component table
 */
#define RUKEN_DEFINE_TAG_COMPONENT(in_component_name)\
    using RUKEN_GLUE(in_component_name, Component) = TagComponent<static_cast<::RUKEN_NAMESPACE::RkSize>(EComponentTable::in_component_name)>

END_RUKEN_NAMESPACE