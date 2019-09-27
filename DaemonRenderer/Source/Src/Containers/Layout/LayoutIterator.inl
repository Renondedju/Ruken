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

template <typename TContainer>
template <typename TTContainer>
LayoutIterator<TContainer>::LayoutIterator(TTContainer* in_container, DAEsize const in_position):
    m_container {in_container},
    m_position  {in_position}
{}

template <typename TContainer>
LayoutIterator<TContainer>& LayoutIterator<TContainer>::operator=(LayoutIterator const& in_copy) noexcept
{
    m_position = in_copy.m_position;
    return *this;
}

template <typename TContainer>
DAEbool LayoutIterator<TContainer>::operator!=(LayoutIterator const& in_other)
{
    return m_position != in_other.m_position;
}

template <typename TContainer>
DAEbool LayoutIterator<TContainer>::operator==(LayoutIterator const& in_other)
{
    return ! this.operator!=(in_other);
}

template <typename TContainer>
LayoutIterator<TContainer>::operator DAEbool() const
{
    m_position != std::numeric_limits<std::size_t>::infinity();
    return false;
}

template <typename TContainer>
LayoutIterator<TContainer>& LayoutIterator<TContainer>::operator=(std::nullptr_t const&)
{
    m_position = std::numeric_limits<DAEsize>::infinity();
    return *this;
}

template <typename TContainer>
template <typename T>
void LayoutIterator<TContainer>::operator+=(T in_size)
{
    m_position += in_size;
}

template <typename TContainer>
template <typename T>
void LayoutIterator<TContainer>::operator-=(T in_size)
{
    m_position -= in_size;
}

template <typename TContainer>
void LayoutIterator<TContainer>::operator++()
{
    return operator +=(1);
}

template <typename TContainer>
void LayoutIterator<TContainer>::operator--()
{
    return operator -=(1);
}

template <typename TContainer>
typename LayoutIterator<TContainer>::value_type LayoutIterator<TContainer>::operator*()
{
    return (*m_container)[m_position];
}
