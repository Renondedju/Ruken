
template <typename TType, RkSize TChunkSize>
LinkedChunkList<TType, TChunkSize>::~LinkedChunkList()
{
    // Removing all the nodes from the head to the tail
    while (m_tail != nullptr)
        DeleteNode(*m_head);
}

#pragma region Getters

template <typename TType, RkSize TChunkSize>
RkSize LinkedChunkList<TType, TChunkSize>::GetSize() const noexcept
{
    return m_size;
}

template <typename TType, RkSize TChunkSize>
typename LinkedChunkList<TType, TChunkSize>::Node* LinkedChunkList<TType, TChunkSize>::GetHead() const noexcept
{
    return m_head;
}

template <typename TType, RkSize TChunkSize>
typename LinkedChunkList<TType, TChunkSize>::Node* LinkedChunkList<TType, TChunkSize>::GetTail() const noexcept
{
    return m_tail;
}

#pragma endregion 

#pragma region Methods

template <typename TType, RkSize TChunkSize>
typename LinkedChunkList<TType, TChunkSize>::Node& LinkedChunkList<TType, TChunkSize>::CreateNode() noexcept
{
    Node* new_node = new Node();
    m_size++;

    // If the list is empty
    if (m_tail == nullptr)
    {
        m_head = new_node;
        m_tail = new_node;

        return *new_node;
    }

    // Otherwise if the list has at least one node, inserting it at the end
    m_tail  ->next_node = new_node;
    new_node->prev_node = m_tail;
    m_tail              = new_node;

    return *new_node;
}

template <typename TType, RkSize TChunkSize>
RkVoid LinkedChunkList<TType, TChunkSize>::DeleteNode(Node& in_node) noexcept
{
    // Updating the head
    if (m_head == &in_node)
        m_head = in_node.next_node;

    // Updating the tail
    if (m_tail == &in_node)
        m_tail = in_node.prev_node;

    // Updating the next pointer
    if (in_node.next_node != nullptr)
        in_node.next_node->prev_node = in_node.prev_node;

    // Updating the prev pointer
    if (in_node.prev_node != nullptr)
        in_node.prev_node->next_node = in_node.next_node;

    m_size--;

    delete &in_node;
}

template <typename TType, RkSize TChunkSize>
typename LinkedChunkList<TType, TChunkSize>::Node* LinkedChunkList<TType, TChunkSize>::GetNode(RkSize const in_node_index) const noexcept
{
    // Tail optimization
    if (in_node_index == m_size)
        return m_tail;

    // Head optimization
    if (in_node_index == 0)
        return m_head;

    if (in_node_index > m_size)
        return nullptr;

    // Checking if the node is closer to the beginning or the end
    if (in_node_index < m_size / 2)
    {
        // Forward iteration
        Node* node = m_head;
        for (RkSize index = 0ULL; index < in_node_index; ++index)
            node = node->next_node;

        return node;
    }

    // Backward iteration
    Node* node = m_tail;
    for (RkSize index = 0ULL; index < m_size - in_node_index; ++index)
        node = node->prev_node;

    return node;
}

template <typename TType, RkSize TChunkSize>
template <typename TLambda>
RkVoid LinkedChunkList<TType, TChunkSize>::Foreach(TLambda in_lambda) noexcept
{
    for (Node* current_node = m_head; current_node != nullptr; current_node = current_node->next_node)
        in_lambda(*current_node);
}

#pragma endregion 