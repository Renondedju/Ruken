
#include "Threading/Worker.hpp"

USING_RUKEN_NAMESPACE

Worker::~Worker() noexcept
{
    if (m_thread.joinable())
        m_thread.join();
}

RkBool Worker::Available() const noexcept
{
    return m_thread.joinable();
}

RkVoid Worker::WaitForAvailability() noexcept
{
    if (m_thread.joinable())
        m_thread.join();
}

RkVoid Worker::Detach() noexcept
{
    if (!m_thread.joinable())
        m_thread.detach();
}

std::thread& Worker::Thread() noexcept
{
    return m_thread;
}

std::thread::id Worker::ID() const noexcept
{
    return m_thread.get_id();
}
