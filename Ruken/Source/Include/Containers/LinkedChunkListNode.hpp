
#pragma once

#include <array>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Represents a node in a LinkedChunkList
 * \tparam TType Elements type of the node
 * \tparam TChunkSize Size in octets of the chunk (node) (default is 32Mb or 32768 octets)
 */
template<typename TType, RkSize TChunkSize = 32768>
class LinkedChunkListNode
{
    public:

        static constexpr RkSize element_count = TChunkSize / sizeof(TType);

        #pragma region Members

        std::array<TType, element_count> data      {};
        LinkedChunkListNode*             next_node {nullptr};
        LinkedChunkListNode*             prev_node {nullptr};

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

END_RUKEN_NAMESPACE