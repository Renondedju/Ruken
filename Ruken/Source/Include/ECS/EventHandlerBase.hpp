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

#include "Build/Namespace.hpp"
#include "ECS/ComponentQuery.hpp"
#include "ECS/EEventName.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Base class of every event handler
 *        Used to store handlers by reference or pointer without having to deal with templates
 */
class EventHandlerBase
{
    protected:

        #pragma region Members

        ComponentQuery m_query {};

        #pragma endregion

    public:

        #pragma region Constructors

        EventHandlerBase()                                = default;
        EventHandlerBase(EventHandlerBase const& in_copy) = default;
        EventHandlerBase(EventHandlerBase&&      in_move) = default;
        virtual ~EventHandlerBase()                       = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the component query of the event handler
         * \return Component query
         */
        ComponentQuery const& GetQuery() const noexcept;

        /**
         * \brief Returns the name of the handled event
         * \return Event name
         */
        virtual EEventName GetHandledEvent() noexcept = 0;

        /**
         * \brief Adds an archetype reference to the event handler.
         * \note The passed archetype must be compatible with the event handler component query
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        virtual RkVoid AddArchetypeReference(Archetype& in_archetype) noexcept = 0;

        /**
         * \brief Runs the event handler
         */
        virtual RkVoid Execute() noexcept = 0;

        #pragma endregion

        #pragma region Operators

        EventHandlerBase& operator=(EventHandlerBase const& in_copy) = default;
        EventHandlerBase& operator=(EventHandlerBase&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE