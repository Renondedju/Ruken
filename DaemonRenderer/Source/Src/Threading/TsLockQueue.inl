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

template<typename TData_Type>
TsLockQueue<TData_Type>::TsLockQueue():
	m_empty_notification {},
	m_push_notification	 {},
	m_unlock_all		 {false},
	m_queue				 {}
{}

template<typename TData_Type>
TsLockQueue<TData_Type>::~TsLockQueue()
{
	Release();
}

template<typename TData_Type>
template<typename... TData, typename>
DAEvoid TsLockQueue<TData_Type>::Push(TData&... in_data)
{
	{
		decltype(m_queue)::WriteAccess access(m_queue);

		access->push(TData_Type(in_data));
	}

	m_push_notification.notify_one();
}

template<typename TData_Type>
TData_Type TsLockQueue<TData_Type>::Pop()
{
	decltype(m_queue)::WriteAccess access(m_queue);
	
	// if the queue is empty waiting for a new data to pop
	m_push_notification.wait(access.GetLock(), []() {
		return m_queue.empty() && !m_unlock_all.load(std::memory_order_acquire);
	});

	// If the while above has been interrupted by the release() method and the queue is empty, returning a default data to quit the method.
	if (m_queue.empty())
	{
		m_empty_notification.notify_all();
		return TData_Type();
	}

	// Popping a new data
	TData_Type data = m_queue.front();
	m_queue.pop();

	// If the queue is empty, notifying the waitUntilEmpty() method
	if (m_queue.empty())
		m_empty_notification.notify_all();

	return data;
}

template<typename TData_Type>
DAEvoid TsLockQueue<TData_Type>::Release()
{
	m_unlock_all.store(true, std::memory_order_release);
	m_push_notification.notify_all();
}

template<typename TData_Type>
DAEbool TsLockQueue<TData_Type>::isEmpty() noexcept
{
	decltype(m_queue)::ReadAccess access(m_queue);
	return access->empty();
}

template<typename TData_Type>
DAEvoid TsLockQueue<TData_Type>::WaitUntilEmpty()
{
	if (isEmpty())
		return;

	std::unique_lock<std::mutex> lock(m_emptyMutex);

	while (!isEmpty())
		m_empty_notification.wait(lock);
}
