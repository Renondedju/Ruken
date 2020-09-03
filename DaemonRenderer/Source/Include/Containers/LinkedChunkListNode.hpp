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

#include <array>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Represents a node in a LinkedChunkList
 * \tparam TType Elements type of the node
 * \tparam TChunkSize Size in octets of the chunk (node) (default is 16Kb or 2046 octets)
 */
template<typename TType, DAEsize TChunkSize = 2048>
class LinkedChunkListNode
{
    public:

        #pragma region Members

        std::array<TType, TChunkSize / sizeof(TType)> data      {};
        LinkedChunkListNode<TType, TChunkSize>*       next_node {nullptr};
        LinkedChunkListNode<TType, TChunkSize>*       prev_node {nullptr};

        #pragma endregion

        #pragma region Constructors

        LinkedChunkListNode()                                   = default;
        LinkedChunkListNode(LinkedChunkListNode const& in_copy) = default;
        LinkedChunkListNode(LinkedChunkListNode&&      in_move) = default;
        ~LinkedChunkListNode()                                  = default;

        #pragma endregion

        #pragma region Operators

        LinkedChunkListNode& operator=(LinkedChunkListNode const& in_copy) = default;
        LinkedChunkListNode& operator=(LinkedChunkListNode&&      in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE