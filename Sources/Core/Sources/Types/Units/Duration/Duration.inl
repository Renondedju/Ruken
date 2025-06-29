
#pragma once

#pragma region Operators

template <>
constexpr Duration<EDurationUnit::Second>::operator Duration<EDurationUnit::Minute>() const noexcept
{
    return Duration<EDurationUnit::Minute>(static_cast<UnderlyingType>(*this) / 60.0F);
}

template <>
constexpr Duration<EDurationUnit::Millisecond>::operator Duration<EDurationUnit::Minute>() const noexcept
{
    return Duration<EDurationUnit::Minute>(static_cast<UnderlyingType>(*this) / 60'000.0F);
}

template <>
constexpr Duration<EDurationUnit::Minute>::operator Duration<EDurationUnit::Second>() const noexcept
{
    return Duration<EDurationUnit::Second>(static_cast<UnderlyingType>(*this) * 60.0F);
}

template <>
constexpr Duration<EDurationUnit::Millisecond>::operator Duration<EDurationUnit::Second>() const noexcept
{
    return Duration<EDurationUnit::Second>(static_cast<UnderlyingType>(*this) / 1000.0F);
}

template <>
constexpr Duration<EDurationUnit::Second>::operator Duration<EDurationUnit::Millisecond>() const noexcept
{
    return Duration<EDurationUnit::Millisecond>(static_cast<UnderlyingType>(*this) * 1000.0F);
}

template <>
constexpr Duration<EDurationUnit::Minute>::operator Duration<EDurationUnit::Millisecond>() const noexcept
{
    return Duration<EDurationUnit::Millisecond>(static_cast<UnderlyingType>(*this) * 60'000.0F);
}

// Type literals

constexpr Milliseconds operator"" _ms(RkLdouble const in_duration) noexcept
{
    return Milliseconds(static_cast<Milliseconds::UnderlyingType>(in_duration));
}

constexpr Milliseconds operator"" _ms(RkSize const in_duration) noexcept
{
    return Milliseconds(static_cast<Milliseconds::UnderlyingType>(in_duration));
}

constexpr Seconds operator"" _sec(RkLdouble const in_duration) noexcept
{
    return Seconds(static_cast<Seconds::UnderlyingType>(in_duration));
}

constexpr Seconds operator"" _sec(RkSize const in_duration) noexcept
{
    return Seconds(static_cast<Seconds::UnderlyingType>(in_duration));
}

constexpr Minutes operator"" _min(RkLdouble const in_duration) noexcept
{
    return Minutes(static_cast<Minutes::UnderlyingType>(in_duration));
}

constexpr Minutes operator"" _min(RkSize const in_duration) noexcept
{
    return Minutes(static_cast<Minutes::UnderlyingType>(in_duration));
}

#pragma endregion
