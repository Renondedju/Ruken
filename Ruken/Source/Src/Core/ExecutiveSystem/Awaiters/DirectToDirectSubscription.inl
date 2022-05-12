#pragma once

#pragma region Constructors

template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue> requires DirectToDirectQueue<TOwningQueue, TAwaitedQueue>
Subscription<TOwningQueue, TAwaitedQueue>::Subscription(Task<TOwningQueue> const& in_owning, AsynchronousEvent<TAwaitedQueue>& in_awaited) noexcept:
    DirectSubscriptionBase {in_awaited.m_head},
    m_owning               {in_owning}
{}

#pragma endregion

#pragma region Methods

template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue> requires DirectToDirectQueue<TOwningQueue, TAwaitedQueue>
RkVoid Subscription<TOwningQueue, TAwaitedQueue>::OnCompletion() noexcept
{
    TOwningQueue::Enqueue(m_owning);
}

#pragma endregion
