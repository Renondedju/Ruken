
template <typename TExecutable, typename ...TArgs>
RkVoid Worker::Execute(TExecutable in_job, TArgs... in_args) noexcept
{
    // This avoids an std::terminate throw
    WaitForAvailability();

    m_thread = std::thread {in_job, in_args...};
}

template <typename TExecutable, typename ...TArgs>
RkVoid Worker::ExecuteWithInstance(TExecutable in_job, TArgs... in_args) noexcept
{
    // This avoids an std::terminate throw
    WaitForAvailability();

    m_thread = std::thread{in_job, in_args..., this};
}