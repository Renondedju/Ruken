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

#pragma region Constructors

template <typename TEnum_Type>
constexpr Bitmask<TEnum_Type>::Bitmask(Underlying_Type in_data) noexcept:
    m_data {in_data}
{}

template <typename TEnum_Type>
template <typename... TData, typename>
constexpr Bitmask<TEnum_Type>::Bitmask(TData... in_data) noexcept:
    m_data {static_cast<Underlying_Type>(0)}
{
    m_data = ((1 << static_cast<Underlying_Type>(in_data)) | ...);
}

template <typename TEnum_Type>
constexpr Bitmask<TEnum_Type>::Bitmask() noexcept:
    m_data {static_cast<Underlying_Type>(0)}
{}

#pragma endregion

#pragma region Methods

template <typename TEnum_Type>
template <typename... TData, typename>
constexpr DAEbool Bitmask<TEnum_Type>::HasAll(TData... in_data) const noexcept
{
    Underlying_Type bitmask = ((1 << static_cast<Underlying_Type>(in_data)) | ...);
    return ((m_data & bitmask) == bitmask);
}

template <typename TEnum_Type>
constexpr DAEbool Bitmask<TEnum_Type>::HasAll(Bitmask<TEnum_Type> const& in_bitmask) const noexcept
{
    return ((m_data & in_bitmask.m_data) == in_bitmask.m_data);
}

template <typename TEnum_Type>
template <typename... TData, typename>
constexpr DAEbool Bitmask<TEnum_Type>::HasOne(TData... in_data) const noexcept
{
    return m_data & ((1 << static_cast<Underlying_Type>(in_data)) | ...);
}

template <typename TEnum_Type>
constexpr DAEbool Bitmask<TEnum_Type>::HasOne(Bitmask<TEnum_Type> const& in_bitmask) const noexcept
{
    return m_data & in_bitmask.m_data;
}

template <typename TEnum_Type>
template <typename... TData, typename>
constexpr DAEvoid Bitmask<TEnum_Type>::Add(TData... in_data) noexcept
{
    m_data |= ((1 << static_cast<Underlying_Type>(in_data)) | ...);
}

template<typename TEnum_Type>
constexpr DAEvoid Bitmask<TEnum_Type>::Add(Bitmask<TEnum_Type> const& in_bitmask) noexcept
{
    m_data |= in_bitmask.m_data;
}

template <typename TEnum_Type>
template <typename... TData, typename>
constexpr DAEvoid Bitmask<TEnum_Type>::Remove(TData... in_data) noexcept
{
    m_data &= static_cast<Underlying_Type>(~(((1 << static_cast<Underlying_Type>(in_data))) | ...));
}

template <typename TEnum_Type>
constexpr DAEvoid Bitmask<TEnum_Type>::Remove(Bitmask<TEnum_Type> const& in_bitmask) noexcept
{
    m_data &= ~in_bitmask.m_data;
}

template <typename TEnum_Type>
constexpr DAEvoid Bitmask<TEnum_Type>::Clear() noexcept
{
    m_data = 0;
}

template <typename TEnum_Type>
constexpr DAEuint16 Bitmask<TEnum_Type>::Enabled() const noexcept
{
    DAEuint16 count = 0u;
    Underlying_Type data = m_data;

    while (data) 
    { 
        data &= (data - 1);
        count++;
    }

    return count;
}

template <typename TEnum_Type>
template <typename TLambda_Type>
constexpr DAEvoid Bitmask<TEnum_Type>::Foreach(TLambda_Type in_lambda) const noexcept
{
    for (DAEint16 index = 0; index < sizeof(Underlying_Type) * 8; index++)
        if ((1 << index) & m_data)
            in_lambda(static_cast<TEnum_Type>(index));
}

template <typename TEnum_Type>
constexpr typename Bitmask<TEnum_Type>::Underlying_Type Bitmask<TEnum_Type>::Data() const noexcept
{
    return m_data;
}

template <typename TEnum_Type>
constexpr typename Bitmask<TEnum_Type>::Underlying_Type& Bitmask<TEnum_Type>::Data() noexcept
{
    return m_data;
}

#pragma endregion

#pragma region Operators

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type> Bitmask<TEnum_Type>::operator+ (TEnum_Type const& in_bit) const noexcept
{
    return Bitmask<TEnum_Type>(m_data | (1 << static_cast<Underlying_Type>(in_bit)));
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type> Bitmask<TEnum_Type>::operator+ (Bitmask<TEnum_Type> const& in_bitmask) const noexcept
{
    return Bitmask<TEnum_Type>(m_data | in_bitmask.m_data);
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type>& Bitmask<TEnum_Type>::operator+=(Bitmask<TEnum_Type> const& in_bitmask) noexcept
{
    m_data |= in_bitmask.m_data;
    return *this;
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type> Bitmask<TEnum_Type>::operator+=(TEnum_Type const& in_bit) noexcept
{
    m_data |= 1 << static_cast<Underlying_Type>(in_bit);
    return *this;
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type> Bitmask<TEnum_Type>::operator- (TEnum_Type const& in_bit) const noexcept
{
    return Bitmask<TEnum_Type>(m_data - (1 << static_cast<Underlying_Type>(in_bit)));
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type> Bitmask<TEnum_Type>::operator- (Bitmask<TEnum_Type> const& in_bitmask) const noexcept
{
    return Bitmask<TEnum_Type>(m_data - in_bitmask.m_data);
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type>& Bitmask<TEnum_Type>::operator-=(TEnum_Type const& in_bit) noexcept
{
    m_data &= ~(1 << static_cast<Underlying_Type>(in_bit));

    return *this;
}

template<typename TEnum_Type>
constexpr Bitmask<TEnum_Type>& Bitmask<TEnum_Type>::operator-=(Bitmask<TEnum_Type> const& in_bitmask) noexcept
{
    m_data &= ~in_bitmask.m_data;
    return *this;
}

template<typename TEnum_Type>
constexpr DAEbool Bitmask<TEnum_Type>::operator==(Bitmask<TEnum_Type> const& in_bitmask) const noexcept
{
    return m_data == in_bitmask.m_data;
}

#pragma endregion

template<typename TEnum_Type>
std::ostream& operator<<(std::ostream& out_stream, Bitmask<TEnum_Type> const& in_bitmask)
{
    for (DAEint16 i = sizeof(typename Bitmask<TEnum_Type>::Underlying_Type) * 8 - 1; i >= 0; --i)
    {
        if ((i + 1) % 4 == 0)
            out_stream << ' ';

        out_stream << ((static_cast<typename Bitmask<TEnum_Type>::Underlying_Type>((1 << i)) & in_bitmask.Data()) ? '1' : '0');
    }

    return out_stream;
}