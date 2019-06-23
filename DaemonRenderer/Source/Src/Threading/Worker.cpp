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

#include "Threading/Worker.hpp"

USING_DAEMON_NAMESPACE

// Label enabled code
#ifdef DAEMON_THREADING_ENABLE_THREAD_LABELS

Worker::Worker(Bitmask<EWorkerFlag> in_flags,
			   DAEchar const*		in_label) noexcept:
	m_thread {},
	m_flags	 {in_flags},
	m_label  {in_label}
{}

String const& Worker::Label() const noexcept
{
	return m_label;
}

#else // Label disabled code

Worker::Worker(Bitmask<EWorkerFlag> in_flags,
			   DAEchar const*) noexcept:
	m_thread {},
	m_flags	 {in_flags}
{}

String Worker::Label() const noexcept
{
	return "";
}

#endif

Worker::~Worker() noexcept
{
	if (m_thread.joinable())
		m_thread.join();
}

Bitmask<EWorkerFlag> Worker::Flags() const noexcept
{
	return m_flags;
}

DAEbool Worker::Available() const noexcept
{
	return m_thread.joinable();
}

DAEvoid Worker::WaitForAvailability() noexcept
{
	if (m_thread.joinable())
		m_thread.join();
}

std::thread& Worker::Thread() noexcept
{
	return m_thread;
}
