
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Containers/LinkedChunkListNode.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A linked chunk list mimics a linked list but instead of every node containing one object,
 *        it contains multiple ones, by default 16Kb of them
 * \tparam TType Type to hold in every node
 * \tparam TChunkSize Size in octets of one chunk (default is 16Kb or 2046 octets)
 */
template<typename TType, RkSize TChunkSize = 2048>
class LinkedChunkList
{
    public:

        using Node = LinkedChunkListNode<TType, TChunkSize>;

        static constexpr RkSize chunk_element_count = Node::element_count;

    private:

        #pragma region Members

        Node*  m_head {nullptr};
        Node*  m_tail {nullptr};
        RkSize m_size {0ULL};

        #pragma endregion

    public:

        #pragma region Constructors

        LinkedChunkList()                               = default;
        LinkedChunkList(LinkedChunkList const& in_copy) = default;
        LinkedChunkList(LinkedChunkList&&      in_move) = default;
        ~LinkedChunkList();

        #pragma endregion

        #pragma region Getters

        /**
         * \brief Returns the number of chunks stored in the list
         * \return Number of chunks stored in the list
         */
        [[nodiscard]] RkSize GetSize() const noexcept;

        /**
         * \brief Returns the head of the list, could be null 
         * \return Head of the list
         */
        [[nodiscard]] Node* GetHead() const noexcept;

        /**
         * \brief Returns the tail of the list, could be null
         * \return Tail of the list
         */
        [[nodiscard]] Node* GetTail() const noexcept;

        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Creates a new node at the end of the list
         * \return Reference onto the new node
         */
        Node& CreateNode() noexcept;

        /**
         * \brief Deletes a node from the list 
         * \param in_node Reference onto the node to delete
         * \warning Make sure the node you are passing is owned by the list you are calling this function on
         *          otherwise, that may create bugs in ur code, since no verification is done before manipulating the node
         */
        RkVoid DeleteNode(Node& in_node) noexcept;

        /**
         * \brief Finds a node at a given index and returns it
         * \param in_node_index Index of the node to return
         * \return Node or nullptr if the requested node does not exist
         */
        Node* GetNode(RkSize in_node_index) const noexcept;

        /**
         * \brief Executes a function for each element block of the list
         * \tparam TLambda Lambda type, of type void (*in_lambda)(LinkedChunkList<TType, TChunkSize>::Node&)
         * \param in_lambda Actual lambda
         */
        template <typename TLambda>
        RkVoid Foreach(TLambda in_lambda) noexcept;

        #pragma endregion

        #pragma region Operators

        LinkedChunkList& operator=(LinkedChunkList const& in_copy) = default;
        LinkedChunkList& operator=(LinkedChunkList&&      in_move) = default;

        #pragma endregion
};

#include "Containers/LinkedChunkList.inl"

END_RUKEN_NAMESPACE
