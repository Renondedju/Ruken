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

template <DAEsize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr SizedBitmask<TSize, TChunk>::SizedBitmask(TData... in_data) noexcept:
    m_data {}
{
    Add(in_data...);
}


// --- Methods

template <DAEsize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr DAEbool SizedBitmask<TSize, TChunk>::HasAll(TData... in_data) const noexcept
{
    return (((m_data[in_data / sizeof_chunk] & TChunk(1) << (in_data % sizeof_chunk)) == TChunk(1) << (in_data % sizeof_chunk)) && ...);
}

template <DAEsize TSize, typename TChunk>
constexpr DAEbool SizedBitmask<TSize, TChunk>::HasAll(SizedBitmask const& in_bitmask) const noexcept
{
    for (DAEsize index = 0; index < TSize; ++index)
        if (!((m_data[index] & in_bitmask.m_data[index]) == in_bitmask.m_data[index]))
            return false;

    return true;
}

template <DAEsize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr DAEbool SizedBitmask<TSize, TChunk>::HasOne(TData... in_data) const noexcept
{
    return ((m_data[in_data / sizeof_chunk] & TChunk(1) << (in_data % sizeof_chunk)) || ...);
}

template <DAEsize TSize, typename TChunk>
constexpr DAEbool SizedBitmask<TSize, TChunk>::HasOne(SizedBitmask const& in_bitmask) const noexcept
{
    for (DAEsize index = 0; index < TSize; ++index)
        if ((m_data[index] & in_bitmask.m_data[index]))
            return true;

    return false;
}

template <DAEsize TSize, typename TChunk>
constexpr DAEuint16 SizedBitmask<TSize, TChunk>::Popcnt() const noexcept
{
    // I know that __popcnt16, __popcnt, __popcnt64 is a thing but I don't want to be dependent
    // of the ABM instruction set since it has been dropped on recent modern AMD CPUs
    // and isn't supported by many other processors such as ARM CPUs.

    DAEuint16 count = 0u;

    for (DAEsize index = 0; index < TSize; ++index)
    {
        TChunk data = m_data[index];
        while (data) 
        { 
            data &= (data - 1);
            count++;
        }
    }

    return count;
}

template <DAEsize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr DAEvoid SizedBitmask<TSize, TChunk>::Add(TData... in_data) noexcept
{
    ((m_data[in_data / sizeof_chunk] |= (TChunk(1) << (in_data % sizeof_chunk))), ...);
}

template <DAEsize TSize, typename TChunk>
constexpr DAEvoid SizedBitmask<TSize, TChunk>::Add(SizedBitmask const& in_bitmask) noexcept
{
    for(DAEsize index = 0; index < TSize; ++index)
        m_data[index] |= in_bitmask.m_data[index];
}

template <DAEsize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr DAEvoid SizedBitmask<TSize, TChunk>::Remove(TData... in_data) noexcept
{
    ((m_data[in_data / sizeof_chunk] &= ~(TChunk(1) << (in_data % sizeof_chunk))), ...);
}

template <DAEsize TSize, typename TChunk>
constexpr DAEvoid SizedBitmask<TSize, TChunk>::Remove(SizedBitmask const& in_bitmask) noexcept
{
    for (DAEsize index = 0; index < TSize; ++index)
        m_data[index] &= ~in_bitmask.m_data[index];
}

template <DAEsize TSize, typename TChunk>
constexpr DAEvoid SizedBitmask<TSize, TChunk>::Clear() noexcept
{
    memset(m_data, 0, sizeof(m_data));
}

template <DAEsize TSize, typename TChunk>
template <typename TLambdaType, typename TPreCast>
constexpr DAEvoid SizedBitmask<TSize, TChunk>::Foreach(TLambdaType in_lambda) const noexcept
{
    for (DAEsize index = 0; index < TSize; ++index)
        for (DAEsize sub_index = 0; sub_index < sizeof_chunk; ++sub_index)
            if ((TChunk(1) << sub_index) & m_data[index])
                in_lambda(static_cast<TPreCast>(index * sizeof_chunk + sub_index));
}

// --- Operators

template <DAEsize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk> SizedBitmask<TSize, TChunk>::operator+(SizedBitmask const& in_bitmask) const noexcept
{
    return SizedBitmask<TSize, TChunk>(m_data | in_bitmask.m_data);
}

template <DAEsize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk> SizedBitmask<TSize, TChunk>::operator-(SizedBitmask const& in_bitmask) const noexcept
{
    return SizedBitmask<TSize, TChunk>(m_data - in_bitmask.m_data);
}

template <DAEsize TSize, typename TChunk>
constexpr DAEbool SizedBitmask<TSize, TChunk>::operator==(SizedBitmask const& in_bitmask) const noexcept
{
    return m_data == in_bitmask.m_data;
}

template <DAEsize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk>& SizedBitmask<TSize, TChunk>::operator+=(SizedBitmask const& in_bitmask) noexcept
{
    Add(in_bitmask);
    return *this;
}

template <DAEsize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk>& SizedBitmask<TSize, TChunk>::operator-=(SizedBitmask const& in_bitmask) noexcept
{
    Remove(in_bitmask);
    return *this;
}