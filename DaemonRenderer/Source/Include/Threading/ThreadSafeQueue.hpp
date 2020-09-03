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

#include "Config.hpp"

#include "Containers/Queue.hpp"
#include "Threading/Synchronized.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Thread safe queue encapsulation
 * \tparam TType Type contained into the queue
 */
template <typename TType>
class ThreadSafeQueue
{
    private:

        #pragma region Members

        Synchronized<Queue<TType>> m_queue;

        using QueueReadAccess  = decltype(m_queue)::ReadAccess;
        using QueueWriteAccess = decltype(m_queue)::WriteAccess;

        #pragma endregion

    public:

        #pragma region Constructors

        ThreadSafeQueue()                                    = default;
        ThreadSafeQueue(ThreadSafeQueue const& in_copy)        = default;
        ThreadSafeQueue(ThreadSafeQueue&& in_move) noexcept = default;
        ~ThreadSafeQueue()                                    = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the queue is empty
         * \return True if the queue is empty, false otherwise
         */
        DAEbool Empty() const noexcept;

        /**
         * \brief Enqueue an item
         * \param in_item Item to enqueue
         */
        DAEvoid Enqueue(TType&& in_item) noexcept;

        /**
         * \brief Tries to dequeue an item
         * \warning If the queue is empty, the result of this function and the content of out_item are undefined
         * \param out_item Dequeued item
         */
        DAEvoid Dequeue(TType& out_item) noexcept;

        #pragma endregion

        #pragma region Operators

        ThreadSafeQueue& operator=(ThreadSafeQueue const& in_copy)        = default;
        ThreadSafeQueue& operator=(ThreadSafeQueue&& in_move) noexcept    = default;

        #pragma endregion
};

#include "Threading/ThreadSafeQueue.inl"

END_RUKEN_NAMESPACE
