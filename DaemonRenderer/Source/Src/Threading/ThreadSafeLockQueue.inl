/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

template<typename TType>
ThreadSafeLockQueue<TType>::ThreadSafeLockQueue():
	m_empty_notification {},
	m_push_notification	 {},
	m_unlock_all		 {false},
	m_empty_mutex		 {},
	m_queue				 {}
{}

template<typename TType>
ThreadSafeLockQueue<TType>::~ThreadSafeLockQueue()
{
	Release();
}

template<typename TType>
DAEvoid ThreadSafeLockQueue<TType>::Enqueue(TType&& in_item) noexcept
{
	{
		QueueWriteAccess access(m_queue);
		access->push(in_item);
	}

	m_push_notification.notify_one();
}

template<typename TType>
DAEbool ThreadSafeLockQueue<TType>::Dequeue(TType& out_item) noexcept
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
DAEvoid ThreadSafeLockQueue<TType>::Release()
{
	m_unlock_all.store(true, std::memory_order_release);
	m_push_notification.notify_all();
}

template<typename TType>
DAEbool ThreadSafeLockQueue<TType>::Empty() noexcept
{
	QueueReadAccess access(m_queue);
	return access->empty();
}

template<typename TType>
DAEvoid ThreadSafeLockQueue<TType>::WaitUntilEmpty()
{
	if (Empty())
		return;

	std::unique_lock<std::mutex> lock(m_empty_mutex);

	m_empty_notification.wait(lock, [&] {
		return Empty();
	});
}
