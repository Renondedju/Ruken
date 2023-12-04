
#include "Threading/OldWorker.hpp"

USING_RUKEN_NAMESPACE

OldWorker::~OldWorker() noexcept
{
    if (m_thread.joinable())
        m_thread.join();
}

RkBool OldWorker::Available() const noexcept
{
    return m_thread.joinable();
}

RkVoid OldWorker::WaitForAvailability() noexcept
{
    if (m_thread.joinable())
        m_thread.join();
}

RkVoid OldWorker::Detach() noexcept
{
    if (!m_thread.joinable())
        m_thread.detach();
}

std::thread& OldWorker::Thread() noexcept
{
    return m_thread;
}

std::thread::id OldWorker::ID() const noexcept
{
    return m_thread.get_id();
}
