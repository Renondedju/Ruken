
template <typename TType>
RkBool ThreadSafeQueue<TType>::Empty() const noexcept
{
    QueueReadAccess access(m_queue);

    return access->empty();
}

template <typename TType>
RkVoid ThreadSafeQueue<TType>::Enqueue(TType&& in_item) noexcept
{
    QueueWriteAccess access(m_queue);

    access->emplace_back(std::forward<TType>(in_item));
}

template <typename TType>
RkVoid ThreadSafeQueue<TType>::Dequeue(TType& out_item) noexcept
{
    QueueWriteAccess access(m_queue);
    
    out_item = std::move(access->front());
    access->pop_front();
}