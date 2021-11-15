
#pragma once

#pragma region Operators

template <>
constexpr Distance<EDistanceUnit::Centimeters>::operator Distance<EDistanceUnit::Kilometers>() const noexcept
{
    return Distance<EDistanceUnit::Kilometers>(static_cast<UnderlyingType>(*this) / 100'000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Meters>::operator Distance<EDistanceUnit::Kilometers>() const noexcept
{
    return Distance<EDistanceUnit::Kilometers>(static_cast<UnderlyingType>(*this) / 1000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Centimeters>::operator Distance<EDistanceUnit::Meters>() const noexcept
{
    return Distance<EDistanceUnit::Meters>(static_cast<UnderlyingType>(*this) / 100.0F);
}

template <>
constexpr Distance<EDistanceUnit::Kilometers>::operator Distance<EDistanceUnit::Meters>() const noexcept
{
    return Distance<EDistanceUnit::Meters>(static_cast<UnderlyingType>(*this) * 1000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Kilometers>::operator Distance<EDistanceUnit::Centimeters>() const noexcept
{
    return Distance<EDistanceUnit::Centimeters>(static_cast<UnderlyingType>(*this) * 100'000.0F);
}

template <>
constexpr Distance<EDistanceUnit::Meters>::operator Distance<EDistanceUnit::Centimeters>() const noexcept
{
    return Distance<EDistanceUnit::Centimeters>(static_cast<UnderlyingType>(*this) * 100.0F);
}

// Type literals

constexpr Centimeters operator"" _cm(RkLdouble const in_distance) noexcept
{
    return Centimeters(static_cast<Centimeters::UnderlyingType>(in_distance));
}

constexpr Centimeters operator"" _cm(RkSize const in_distance) noexcept
{
    return Centimeters(static_cast<Centimeters::UnderlyingType>(in_distance));
}

constexpr Meters operator"" _m(RkLdouble const in_distance) noexcept
{
    return Meters(static_cast<Meters::UnderlyingType>(in_distance));
}

constexpr Meters operator"" _m(RkSize const in_distance) noexcept
{
    return Meters(static_cast<Meters::UnderlyingType>(in_distance));
}

constexpr Kilometers operator"" _km(RkLdouble const in_distance) noexcept
{
    return Kilometers(static_cast<Kilometers::UnderlyingType>(in_distance));
}

constexpr Kilometers operator"" _km(RkSize const in_distance) noexcept
{
    return Kilometers(static_cast<Kilometers::UnderlyingType>(in_distance));
}

#pragma endregion