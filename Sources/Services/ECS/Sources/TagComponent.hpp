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

#include "Build/Namespace.hpp"

#include "ECS/ComponentBase.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A tag component does not hold any data, its only purpose is to differentiate 2 entities from each other by "tagging" it.
 */
class TagComponent: public ComponentBase
{
    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_owning_archetype Owning archetype pointer
         */
        TagComponent(Archetype const* in_owning_archetype) noexcept;

        TagComponent(TagComponent const& in_copy) = default;
        TagComponent(TagComponent&&      in_move) = default;
        ~TagComponent() override                  = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \note Since a tag component does not contain any data, this method does nothing
         *
         * \brief Ensures that the component has enough storage space for a given amount of entities
         *        If this is not the case, containers will be allocated
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the component layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        RkSize EnsureStorageSpace(RkSize in_size) noexcept override;

        #pragma endregion 

        #pragma region Operators

        TagComponent& operator=(TagComponent const& in_copy) = default;
        TagComponent& operator=(TagComponent&&      in_move) = default;

        #pragma endregion
};

/**
 * \brief Shorthand to declare a tag component named "in_component_name"
 * \param in_component_name Name of the component as defined in the component table
 */
#define RUKEN_DECLARE_TAG_COMPONENT(in_component_name) struct in_component_name final: TagComponent\
    { using TagComponent::TagComponent; using TagComponent::operator=; RUKEN_DEFINE_COMPONENT_ID_DECLARATION }

END_RUKEN_NAMESPACE