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

template <typename TType>
template <typename ...TArgs, typename>
Synchronized<TType>::Synchronized(TArgs... in_args):
    m_mutex {},
    m_value {std::forward<TArgs>(in_args)...}
{}

template <typename TType>
Synchronized<TType>::Synchronized() noexcept:
    m_mutex {},
    m_value {}
{}

template <typename TType>
Synchronized<TType>::Synchronized(Synchronized const& in_copy) noexcept:
    m_mutex {},
    m_value {in_copy.m_value}
{}

template <typename TType>
Synchronized<TType>::Synchronized(Synchronized&& in_move) noexcept:
    m_mutex {},
    m_value {std::forward(in_move.m_value)}
{}

template <typename TType>
TType const& Synchronized<TType>::Unsafe() const noexcept
{
    return m_value;
}

template <typename TType>
TType& Synchronized<TType>::Unsafe() noexcept
{
    return m_value;
}