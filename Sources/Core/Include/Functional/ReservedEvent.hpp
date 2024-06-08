
#pragma once

#include "Build/Namespace.hpp"
#include "Functional/Event.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Exact same class as the event class except that the Invoke method and () operator are protected.
 * \brief This is meant to be inherited from and friend the owner class (use the macro RUKEN_DECLARE_RESERVED_EVENT)
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
#define RUKEN_DECLARE_RESERVED_EVENT(in_event_name, in_owner_class, ...) class in_event_name : public ReservedEvent<__VA_ARGS__> { friend in_owner_class; }

END_RUKEN_NAMESPACE