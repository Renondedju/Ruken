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
#include "Types/FundamentalTypes.hpp"
#include "Containers/LinkedChunkListNode.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief A linked chunk list mimics a linked list but instead of every node containing one object,
 *        it contains multiple ones, by default 16Kb of them
 * \tparam TType Type to hold in every node
 * \tparam TChunkSize Size in octets of one chunk (default is 16Kb or 2046 octets)
 */
template<typename TType, DAEsize TChunkSize = 2048>
class LinkedChunkList
{
    private:

        #pragma region Members

        LinkedChunkListNode<TType, TChunkSize>* m_head;
        LinkedChunkListNode<TType, TChunkSize>* m_tail;

        #pragma endregion

    public:

        #pragma region Constructors

        LinkedChunkList()                               = default;
        LinkedChunkList(LinkedChunkList const& in_copy) = default;
        LinkedChunkList(LinkedChunkList&&      in_move) = default;
        ~LinkedChunkList()                              = default;

        #pragma endregion

        #pragma region Operators

        LinkedChunkList& operator=(LinkedChunkList const& in_copy) = default;
        LinkedChunkList& operator=(LinkedChunkList&&      in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE