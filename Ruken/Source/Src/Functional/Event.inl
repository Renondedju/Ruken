/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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