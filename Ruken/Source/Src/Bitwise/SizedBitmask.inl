
template <RkSize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr SizedBitmask<TSize, TChunk>::SizedBitmask(TData... in_data) noexcept:
    m_data {}
{
    Add(in_data...);
}


// --- Methods

template <RkSize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr RkBool SizedBitmask<TSize, TChunk>::HasAll(TData... in_data) const noexcept
{
    return (((m_data[in_data / sizeof_chunk] & TChunk(1) << (in_data % sizeof_chunk)) == TChunk(1) << (in_data % sizeof_chunk)) && ...);
}

template <RkSize TSize, typename TChunk>
constexpr RkBool SizedBitmask<TSize, TChunk>::HasAll(SizedBitmask const& in_bitmask) const noexcept
{
    for (RkSize index = 0; index < TSize; ++index)
        if (!((m_data[index] & in_bitmask.m_data[index]) == in_bitmask.m_data[index]))
            return false;

    return true;
}

template <RkSize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr RkBool SizedBitmask<TSize, TChunk>::HasOne(TData... in_data) const noexcept
{
    return ((m_data[in_data / sizeof_chunk] & TChunk(1) << (in_data % sizeof_chunk)) || ...);
}

template <RkSize TSize, typename TChunk>
constexpr RkBool SizedBitmask<TSize, TChunk>::HasOne(SizedBitmask const& in_bitmask) const noexcept
{
    for (RkSize index = 0; index < TSize; ++index)
        if ((m_data[index] & in_bitmask.m_data[index]))
            return true;

    return false;
}

template <RkSize TSize, typename TChunk>
constexpr RkUint16 SizedBitmask<TSize, TChunk>::Popcnt() const noexcept
{
    // I know that __popcnt16, __popcnt, __popcnt64 is a thing but I don't want to be dependent
    // of the ABM instruction set since it has been dropped on recent modern AMD CPUs
    // and isn't supported by many other processors such as ARM CPUs.

    RkUint16 count = 0u;

    for (RkSize index = 0; index < TSize; ++index)
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

template <RkSize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr RkVoid SizedBitmask<TSize, TChunk>::Add(TData... in_data) noexcept
{
    ((m_data[in_data / sizeof_chunk] |= (TChunk(1) << (in_data % sizeof_chunk))), ...);
}

template <RkSize TSize, typename TChunk>
constexpr RkVoid SizedBitmask<TSize, TChunk>::Add(SizedBitmask const& in_bitmask) noexcept
{
    for(RkSize index = 0; index < TSize; ++index)
        m_data[index] |= in_bitmask.m_data[index];
}

template <RkSize TSize, typename TChunk>
template <typename... TData, internal::CheckIntegralTypes<TData...>>
constexpr RkVoid SizedBitmask<TSize, TChunk>::Remove(TData... in_data) noexcept
{
    ((m_data[in_data / sizeof_chunk] &= ~(TChunk(1) << (in_data % sizeof_chunk))), ...);
}

template <RkSize TSize, typename TChunk>
constexpr RkVoid SizedBitmask<TSize, TChunk>::Remove(SizedBitmask const& in_bitmask) noexcept
{
    for (RkSize index = 0; index < TSize; ++index)
        m_data[index] &= ~in_bitmask.m_data[index];
}

template <RkSize TSize, typename TChunk>
constexpr RkVoid SizedBitmask<TSize, TChunk>::Clear() noexcept
{
    memset(m_data, 0, sizeof(m_data));
}

template <RkSize TSize, typename TChunk>
constexpr RkSize SizedBitmask<TSize, TChunk>::HashCode() const noexcept
{
    RkSize hash = 0;
    for (RkSize index = 0; index < TSize; ++index)
        hash ^= m_data[index];

    return hash;
}

template <RkSize TSize, typename TChunk>
template <typename TLambdaType, typename TPreCast>
constexpr RkVoid SizedBitmask<TSize, TChunk>::Foreach(TLambdaType in_lambda) const noexcept
{
    for (RkSize index = 0; index < TSize; ++index)
        for (RkSize sub_index = 0; sub_index < sizeof_chunk; ++sub_index)
            if ((TChunk(1) << sub_index) & m_data[index])
                in_lambda(static_cast<TPreCast>(index * sizeof_chunk + sub_index));
}

// --- Operators

template <RkSize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk> SizedBitmask<TSize, TChunk>::operator+(SizedBitmask const& in_bitmask) const noexcept
{
    return SizedBitmask<TSize, TChunk>(m_data | in_bitmask.m_data);
}

template <RkSize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk> SizedBitmask<TSize, TChunk>::operator-(SizedBitmask const& in_bitmask) const noexcept
{
    return SizedBitmask<TSize, TChunk>(m_data - in_bitmask.m_data);
}

template <RkSize TSize, typename TChunk>
constexpr RkBool SizedBitmask<TSize, TChunk>::operator==(SizedBitmask const& in_other) const noexcept
{
    for (RkSize index = 0; index < TSize; ++index)
        if (m_data[index] != in_other.m_data[index])
            return false;

    return true;
}

template <RkSize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk>& SizedBitmask<TSize, TChunk>::operator+=(SizedBitmask const& in_bitmask) noexcept
{
    Add(in_bitmask);
    return *this;
}

template <RkSize TSize, typename TChunk>
constexpr SizedBitmask<TSize, TChunk>& SizedBitmask<TSize, TChunk>::operator-=(SizedBitmask const& in_bitmask) noexcept
{
    Remove(in_bitmask);
    return *this;
}