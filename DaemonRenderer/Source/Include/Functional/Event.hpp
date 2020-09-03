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

#include <vector>
#include <functional>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Event class. This class is comparable to the C# delegate without the -= operator
 * \tparam TArgs Input arguments of the event
 */
template <typename... TArgs>
class Event
{
    using Function = std::function<DAEvoid(TArgs...)>;

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
        DAEvoid Reset() noexcept;

        /**
         * \brief Subscribes a new function to the event
         * \param in_function Function to subscribe
         */
        DAEvoid Subscribe(Function const& in_function) noexcept;
        DAEvoid Subscribe(Function&&      in_function) noexcept;

        /**
         * \brief Invokes the event
         * \note Every subscriber will be invoked in the same order as they subscribed
         * 
         * \param in_args Arguments to pass to all the subscribers
         */
        DAEvoid Invoke(TArgs... in_args) noexcept;

        #pragma endregion

        #pragma region Operators

        /**
         * \brief Equivalent of the Invoke() method
         * \param in_args Arguments to pass to all the subscribers
         */
        DAEvoid operator()(TArgs... in_args) noexcept;

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