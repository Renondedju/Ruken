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

#include "Meta/Assert.hpp"
#include "Build/Namespace.hpp"

#include "ECS/ComponentBase.hpp"
#include "ECS/Meta/FieldHelper.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Exclusive components are a type of component where only one instance of the component exists in the whole ECS
 * \note Exclusive Components have templates for 2 main reason
 *         - First, it allows for a bit more consistency when declaring components, all you have to do to change a component to an
 *       exclusive component, is add one word in the helper macro declaring your component.
 *         - Secondly, this way, each new specialization of the template will automatically create a new unique id for this component,
 *       avoiding the programmer the pain to having to inherit from this class just to paste the "RUKEN_DEFINE_COMPONENT_ID_DECLARATION" in an empty body
 *       Compilation times might be slowed by a bit, but only when compiling the component for the first time, this does not impact the iteration times
 *
 * \tparam TFields Fields of the component
 */
template <ComponentFieldType... TFields>
class ExclusiveComponent final: public ComponentBase
{
    using Helper = FieldHelper<TFields...>;

    private:

        #pragma region Members

        std::tuple<typename TFields::Type...> m_members;

        #pragma endregion

    public:

        #pragma region Constructors

        ExclusiveComponent() noexcept;
        ExclusiveComponent(ExclusiveComponent const& in_copy) = default;
        ExclusiveComponent(ExclusiveComponent&&      in_move) = default;
        virtual ~ExclusiveComponent() override                = default;

        #pragma endregion

        #pragma region Methods

        RUKEN_DEFINE_COMPONENT_ID_DECLARATION

        /**
         * \note This method is never called since exclusive components do not live in archetypes
         *
         * \brief Ensures that the component has enough storage space for a given amount of entities
         *        If this is not the case, containers will be allocated
         * \param in_size Size to ensure
         * \return Minimum number of elements allocated by one of the containers in the layout
         *         This can be useful to avoid having to call back this function when no new allocation is needed
         */
        [[nodiscard]] 
        virtual RkSize EnsureStorageSpace(RkSize in_size) noexcept override;

        /**
         * \brief Fetches a field from the component
         * \tparam TField Field to fetch
         * \return Reference to the field
         */
        template <ComponentFieldType TField> requires Helper::template FieldExists<TField>::value
        [[nodiscard]] typename TField::Type& Fetch() noexcept
        { return std::get<Helper::template FieldIndex<TField>::value>(m_members); }

        /**
         * \brief Fetches a field from the component
         * \tparam TField Field to fetch
         * \return Constant reference to the field
         */
        template <ComponentFieldType TField> requires Helper::template FieldExists<TField>::value
        [[nodiscard]] typename TField::Type const& Fetch() const noexcept
        { return std::get<Helper::template FieldIndex<TField>::value>(m_members); }

        #pragma endregion

        #pragma region Operators

        ExclusiveComponent& operator=(ExclusiveComponent const& in_copy) = default;
        ExclusiveComponent& operator=(ExclusiveComponent&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ExclusiveComponent.inl"

/**
 * \brief Shorthand to declare an exclusive component named "in_component_name"
 * \param in_component_name Name of the component
 * \param ... Fields of the component. Theses must inherit from the ComponentField class
 */
#define RUKEN_DEFINE_EXCLUSIVE_COMPONENT(in_component_name, ...) using in_component_name = ExclusiveComponent<__VA_ARGS__>

END_RUKEN_NAMESPACE