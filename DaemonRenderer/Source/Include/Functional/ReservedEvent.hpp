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

#include "Config.hpp"
#include "Functional/Event.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Exact same class as the event class except that the Invoke method and () operator are protected.
 * \brief This is meant to be inherited from and friend the owner class (use the macro DAEMON_DECLARE_RESERVED_EVENT)
 * \tparam TArgs 
 */
template <typename... TArgs>
class ReservedEvent : public Event<TArgs...>
{
    protected:

        #pragma region Methods
        
        using Event<TArgs...>::Invoke;
        using Event<TArgs...>::operator();

        #pragma endregion

    public:

        #pragma region Constructors

        ReservedEvent()                             = default;
        ReservedEvent(ReservedEvent const& in_copy) = default;
        ReservedEvent(ReservedEvent&&      in_move) = default;
        ~ReservedEvent()                            = default;
        
        #pragma endregion

        #pragma region Operators

        ReservedEvent& operator=(ReservedEvent const& in_copy) = default;
        ReservedEvent& operator=(ReservedEvent&&      in_move) = default;

        #pragma endregion
};

/**
 * \brief This macro allows for quicker event declarations
 * \param in_event_name Name of the reserved event
 * \param in_owner_class Owner class, only this class will be able to invoke the event
 * \param ... Arguments of the event (types)
 */
#define DAEMON_DECLARE_RESERVED_EVENT(in_event_name, in_owner_class, ...) class in_event_name : public ReservedEvent<__VA_ARGS__> { friend in_owner_class; }

END_DAEMON_NAMESPACE