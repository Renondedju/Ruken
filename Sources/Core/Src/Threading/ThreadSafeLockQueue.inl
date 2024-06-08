
template<typename TType>
ThreadSafeLockQueue<TType>::ThreadSafeLockQueue():
    m_empty_notification {},
    m_push_notification     {},
    m_unlock_all         {false},
    m_empty_mutex         {},
    m_queue                 {}
{}

template<typename TType>
ThreadSafeLockQueue<TType>::~ThreadSafeLockQueue()
{
    Release();
}

template<typename TType>
RkVoid ThreadSafeLockQueue<TType>::Enqueue(TType&& in_item) noexcept
{
    {
        QueueWriteAccess access(m_queue);
        access->push(in_item);
    }

    m_push_notification.notify_one();
}

template<typename TType>
RkBool ThreadSafeLockQueue<TType>::Dequeue(TType& out_item) noexcept
{
    // If the queue is empty waiting for a new data to be queued
    {
        std::unique_lock<std::mutex> push_lock(m_push_mutex);

        m_push_notification.wait(push_lock, [&] {
            return !Empty() || m_unlock_all.load(std::memory_order_acquire);
        });
    }

    // If the wait above has been interrupted by the release() method and the queue is empty, returning here.
    if (m_unlock_all.load(std::memory_order_acquire))
    {
        if (Empty())
            m_empty_notification.notify_all();

        return false;
    }

    QueueWriteAccess access(m_queue);

    // Popping a new data
    out_item = access->front();
    access->pop();

    // If the queue is empty, notifying the waitUntilEmpty() method
    if (access->empty())
        m_empty_notification.notify_all();

    return true;
}

template<typename TType>
RkVoid ThreadSafeLockQueue<TType>::Release()
{
    m_unlock_all.store(true, std::memory_order_release);
    m_push_notification.notify_all();
}

template<typename TType>
RkBool ThreadSafeLockQueue<TType>::Empty() noexcept
{
    QueueReadAccess access(m_queue);
    return access->empty();
}

template<typename TType>
RkVoid ThreadSafeLockQueue<TType>::Clear() noexcept
{
    QueueWriteAccess access(m_queue);
    typename decltype(m_queue)::UnderlyingType().swap(access.Get());

    m_empty_notification.notify_all();
}

template<typename TType>
RkVoid ThreadSafeLockQueue<TType>::WaitUntilEmpty()
{
    if (Empty())
        return;

    std::unique_lock<std::mutex> lock(m_empty_mutex);

    m_empty_notification.wait(lock, [&] {
        return Empty();
    });
}
