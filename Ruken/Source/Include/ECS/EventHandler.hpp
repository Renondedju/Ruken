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

#include "Meta/Assert.hpp"
#include "Meta/PassConst.hpp"
#include "Meta/TupleApply.hpp"
#include "Meta/TupleSubset.hpp"
#include "Meta/UniqueTypes.hpp"

#include "ECS/Group.hpp"
#include "ECS/EEventName.hpp"
#include "ECS/ComponentView.hpp"
#include "ECS/EventHandlerBase.hpp"

#include "ECS/Meta/ComponentHelper.hpp"
#include "ECS/Safety/ComponentType.hpp"
#include "ECS/Safety/TagComponentType.hpp"
#include "ECS/Safety/AnyComponentType.hpp"
#include "ECS/Safety/ComponentFieldType.hpp"
#include "ECS/Safety/ExclusiveComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Handles a single ECS event.
 *        This class allows the ECS to easily add new events and optimize the execution of events by independently define their own needs, allowing for further optimization
 *
 * \tparam TEventName Name of the event to be handled
 * \tparam TComponents Required components for the system to operate
 * \note Passing a component type with a const modifier will notify the ECS that this component is readonly for this event handler
 *       Meaning that the ECS can enforce constant access to every field of the component via the ComponentViews.
 *       This will also notify the ECS update manager that a potential optimization can be done when updating systems,
 *       using multithreading, reducing frame times further along. 
 */
template <EEventName TEventName, AnyComponentType... TComponents>
class EventHandler : public EventHandlerBase
{
    RUKEN_STATIC_ASSERT(UniqueTypes<std::remove_const_t<TComponents>...>::value, "Duplicate components are not allowed and would cause a loss in performance (System::EventHandler)");

    protected:

        #pragma region Usings

        // Tuples containing a specific type of component
        using Components          = typename TupleSubset<IsComponent         , TComponents...>::Type;
        using TagComponents       = typename TupleSubset<IsTagComponent      , TComponents...>::Type;
        using ExclusiveComponents = typename TupleSubset<IsExclusiveComponent, TComponents...>::Type;

        // Component types that should be included in the component query (Archetype query) 
        using QueryComponents = decltype(std::tuple_cat(std::declval<Components>(), std::declval<TagComponents>()));

        /**
         * \brief Checks if the passed field is reachable.
         *        If not, you need to add the owning component of that field to the list of reachable components of the handler (TComponents)
         * \tparam TSelectedField Field to operate the check on
         */
        template <ComponentFieldType TSelectedField>
        using FieldIsReachable = TupleHasType<std::remove_const_t<typename TSelectedField::Component>, std::tuple<std::remove_const_t<TComponents>...>>;

        /**
         * \brief Returns the access type of a passed field
         * \tparam TSelectedField Selected field
         */
        template <ComponentFieldType TSelectedField> requires FieldIsReachable<TSelectedField>::value
        using FieldAccess = CopyConst<
            PassConst<
                std::tuple_element_t<
                    TupleIndex<
                        std::remove_const_t<typename TSelectedField::Component>,
                        std::tuple<std::remove_const_t<TComponents>...>>::value,
                    std::tuple<TComponents...>>,
                TSelectedField>,
            typename TSelectedField::Type>;

        // View type helpers
        template <ComponentFieldType... TSelectedFields> using MakeView         = ComponentView<TSelectedFields...>;
        template <ComponentFieldType... TSelectedFields> using MakeReadonlyView = ComponentView<TSelectedFields const...>;

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
        std::vector<typename TupleApply<Group, Components>::Type> m_groups {};

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
         * \brief Adds a component reference group to the event handler.
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        RkVoid AddReferenceGroup(Archetype& in_archetype) noexcept final;

        /**
         * \brief Returns a reference onto the requested exclusive component
         * \note If this is the first access to the designated exclusive component, this method will allocate the component
         * \tparam TExclusiveComponent exclusive component type
         * \return Exclusive component reference
         *
         * TODO: Implement this
         */
        template <ExclusiveComponentType TExclusiveComponent>
        ExclusiveComponentAccess<TExclusiveComponent>& GetExclusiveComponent() noexcept;

        #pragma endregion

        #pragma region Operators

        EventHandler& operator=(EventHandler const& in_copy) = default;
        EventHandler& operator=(EventHandler&&      in_move) = default;

        #pragma endregion
};

#include "ECS/EventHandler.inl"

END_RUKEN_NAMESPACE
