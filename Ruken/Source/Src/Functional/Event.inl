
template <typename ... TArgs>
RkVoid Event<TArgs...>::Reset() noexcept
{
    m_subscribers.clear();
}

template <typename ... TArgs>
RkVoid Event<TArgs...>::Subscribe(Function const& in_function) noexcept
{
    m_subscribers.emplace_back(std::forward<Function>(in_function));
}

template <typename ... TArgs>
RkVoid Event<TArgs...>::Subscribe(Function&& in_function) noexcept
{
    m_subscribers.emplace_back(std::forward<Function>(in_function));
}

template <typename ... TArgs>
RkVoid Event<TArgs...>::Invoke(TArgs... in_args) noexcept
{
    for (Function& function : m_subscribers)
        function(std::forward<TArgs>(in_args)...);
}

template <typename ... TArgs>
RkVoid Event<TArgs...>::operator()(TArgs... in_args) noexcept
{
    Invoke(std::forward<TArgs>(in_args)...);
}

template <typename ... TArgs>
Event<TArgs...>& Event<TArgs...>::operator+=(Function const& in_function) noexcept
{
    Subscribe(std::forward<Function>(in_function));

    return *this;
}

template <typename ... TArgs>
Event<TArgs...>& Event<TArgs...>::operator+=(Function&& in_function) noexcept
{
    Subscribe(std::forward<Function>(in_function));

    return *this;
}