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
#include <forward_list>

#include "Meta/Assert.hpp"
#include "Meta/PassConst.hpp"
#include "Meta/TupleSubset.hpp"
#include "Meta/UniqueTuple.hpp"
#include "Meta/UniqueTypes.hpp"
#include "Meta/TupleHasType.hpp"

#include "ECS/EEventName.hpp"
#include "ECS/ComponentView.hpp"
#include "ECS/EventHandlerBase.hpp"

#include "ECS/Meta/ComponentHelper.hpp"
#include "ECS/Safety/ComponentType.hpp"
#include "ECS/Safety/TagComponentType.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"
#include "ECS/Safety/ExclusiveComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

template <EEventName TEventName, ComponentFieldType... TFields>
class EventHandler : public EventHandlerBase
{
    RUKEN_STATIC_ASSERT(UniqueTypes<std::remove_const_t<TFields>...>::value, "Duplicate fields are not allowed and would cause a loss in performance");

    #pragma region Usings

    template <ComponentFieldType TField> using FieldChunk    = LinkedChunkListNode<typename TField::Type>;
    template <ComponentFieldType TField> using ReferencePair = std::pair<RkSize, FieldChunk<TField>*>;

    // Tuple containing all the used components of the event handler
    using ComponentList = UniqueTuple<typename TFields::Component...>;

    // Component types
    using Components          = typename TupleSubset<IsComponent         , ComponentList>::Type;
    using TagComponents       = typename TupleSubset<IsTagComponent      , ComponentList>::Type;
    using ExclusiveComponents = typename TupleSubset<IsExclusiveComponent, ComponentList>::Type;

    /**
     * \brief Checks if a field is reachable withing this event handler (pass it in TFields if not)
     * \tparam TField Field to look for
     */
    template <ComponentFieldType TField>
    using FieldIsReachable = TupleHasType<std::remove_const_t<TField>, std::tuple<std::remove_const_t<TFields>...>>;

    /**
     * \brief Returns the constness of the field based on the "global" constness (passed in the TFields of the class)
     *        and the "local" constness of the field (as passed in the TField of this using)
     * \tparam TField Field to evaluate constness for
     */
    template <ComponentFieldType TField> requires FieldIsReachable<TField>::value
    using FieldConstness = PassConst<std::tuple_element_t<TupleIndex<std::remove_const_t<TField>, std::tuple<std::remove_const_t<TFields>...>>::value,std::tuple<TFields...>>, TField>;

    /**
     * \brief Returns the access type of the passed field
     * \tparam TField Field to access
     */
    template <ComponentFieldType TField> requires FieldIsReachable<TField>::value
    using FieldAccess = CopyConst<FieldConstness<TField>, typename TField::Type>;

    #pragma endregion

    protected:

        #pragma region Members

        // Lists all the matching archetypes (via component query)
        std::forward_list<std::reference_wrapper<Archetype>> m_archetypes;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief 
         * \tparam TInvokedFields 
         * \param in_lambda 
         */
        template<typename TFunction, ComponentFieldType... TInvokedFields>
        RkVoid Foreach(TFunction&& in_lambda, Tag<TInvokedFields...>) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        EventHandler() noexcept;
        EventHandler(EventHandler const& in_copy) = default;
        EventHandler(EventHandler&&      in_move) = default;
        ~EventHandler() override                  = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the name of the handled event
         * \return Event name
         */
        EEventName GetHandledEvent() noexcept final;

        /**
         * \brief Adds an archetype reference to the event handler.
         * \note The passed archetype must be compatible with the event handler component query
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        RkVoid AddArchetypeReference(Archetype& in_archetype) noexcept override;

        #pragma endregion

        #pragma region Operators

        EventHandler& operator=(EventHandler const& in_copy) = default;
        EventHandler& operator=(EventHandler&&      in_move) = default;

        #pragma endregion
};

#include "ECS/EventHandler.inl"

END_RUKEN_NAMESPACE
