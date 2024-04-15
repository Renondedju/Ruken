
#pragma once

#include <vector>
#include <functional>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Event class. This class is comparable to the C# delegate without the -= operator
 * \tparam TArgs Input arguments of the event
 */
template <typename... TArgs>
class Event
{
    using Function = std::function<RkVoid(TArgs...)>;

    private:

        #pragma region Members

        std::vector<Function> m_subscribers;

        #pragma endregion 

    public:

        #pragma region Constructors

        Event()                     = default;
        Event(Event const& in_copy) = default;
        Event(Event&&      in_move) = default;
        ~Event()                    = default;
        
        #pragma endregion

        #pragma region Methods

        /**
         * \brief Clears all the subscribers from the event
         */
        RkVoid Reset() noexcept;

        /**
         * \brief Subscribes a new function to the event
         * \param in_function Function to subscribe
         */
        RkVoid Subscribe(Function const& in_function) noexcept;
        RkVoid Subscribe(Function&&      in_function) noexcept;

        /**
         * \brief Invokes the event
         * \note Every subscriber will be invoked in the same order as they subscribed
         * 
         * \param in_args Arguments to pass to all the subscribers
         */
        RkVoid Invoke(TArgs... in_args) noexcept;

        #pragma endregion

        #pragma region Operators

        /**
         * \brief Equivalent of the Invoke() method
         * \param in_args Arguments to pass to all the subscribers
         */
        RkVoid operator()(TArgs... in_args) noexcept;

        /**
         * \brief Equivalent of the Subscribe() method
         * \param in_function Function to subscribe
         * \return Instance of the event
         */
        Event& operator+=(Function const& in_function) noexcept;
        Event& operator+=(Function&&      in_function) noexcept;

        Event& operator=(Event const& in_copy) = default;
        Event& operator=(Event&&      in_move) = default;

        #pragma endregion

};

#include "Functional/Event.inl"

END_RUKEN_NAMESPACE