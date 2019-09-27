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

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
BaseLayoutContainer<TContainer, TDataLayout, TItem>::BaseLayoutContainer(DAEsize const in_size)
{
    Resize(in_size);
}

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
template <typename TFwd>
DAEvoid BaseLayoutContainer<TContainer, TDataLayout, TItem>::PushBack(TFwd&& in_val)
{
    Policy::PushBack(m_values, std::forward<TFwd>(in_val));
}

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
DAEsize BaseLayoutContainer<TContainer, TDataLayout, TItem>::Size()
{
    return Policy::Size(m_values);
}

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
typename BaseLayoutContainer<TContainer, TDataLayout, TItem>::ValueType
    BaseLayoutContainer<TContainer, TDataLayout, TItem>::operator[](DAEsize in_position)
{
    return Policy::Get(m_values, in_position);
}

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
DAEvoid BaseLayoutContainer<TContainer, TDataLayout, TItem>::Resize(DAEsize in_size)
{
    Policy::Resize(m_values, in_size);
}

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
typename BaseLayoutContainer<TContainer, TDataLayout, TItem>::Iterator
    BaseLayoutContainer<TContainer, TDataLayout, TItem>::begin()
{
    return Iterator(this, 0);
}

template <template <typename TValueType> class TContainer, EDataLayout TDataLayout, typename TItem>
typename BaseLayoutContainer<TContainer, TDataLayout, TItem>::Iterator
    BaseLayoutContainer<TContainer, TDataLayout, TItem>::end()
{
    return Iterator(this, Size());
}
