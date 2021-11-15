
#pragma once

#include "Build/Attributes/EmptyBases.hpp"
#include "Types/StrongType/StrongType.hpp"
#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"
#include "Types/StrongType/Operators/Stream.hpp"

#include "Types/Units/Distance/EDistanceUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed distance type
 * \tparam TUnitType Unit type of the distance, must be either Kilometers, Meters or Centimeters
 */
template<EDistanceUnit TUnitType>
struct RUKEN_EMPTY_BASES Distance final:
    StrongType<RkFloat, Distance<TUnitType>>,
    Arithmetic<Distance<TUnitType>>,
    Comparison<Distance<TUnitType>>,
    Stream    <Distance<TUnitType>>
{
    #pragma region Constructors

    using StrongType<RkFloat, Distance<TUnitType>>::StrongType;

    constexpr Distance()                        = default;
    constexpr Distance(Distance const& in_copy) = default;
    constexpr Distance(Distance&&      in_move) = default;
             ~Distance()                        = default;

    #pragma endregion

    #pragma region Operators

    constexpr Distance& operator=(Distance const& in_copy) = default;
    constexpr Distance& operator=(Distance&&      in_move) = default;

    /**
     * \brief Kilometers distance conversion
     */
    [[nodiscard]]
    constexpr operator Distance<EDistanceUnit::Kilometers>() const noexcept;

    /**
     * \brief Meters distance conversion
     */
    [[nodiscard]]
    constexpr operator Distance<EDistanceUnit::Meters>() const noexcept;

    /**
     * \brief Centimeters distance conversion
     */
    [[nodiscard]]
    constexpr operator Distance<EDistanceUnit::Centimeters>() const noexcept;

    #pragma endregion
};

// Type aliases

using Centimeters = Distance<EDistanceUnit::Centimeters>;
using Meters      = Distance<EDistanceUnit::Meters>;
using Kilometers  = Distance<EDistanceUnit::Kilometers>;

// Suffixes

template<>
struct StrongTypeSuffix<Centimeters>
{
    static constexpr const RkChar* suffix = " cm";
};

template<>
struct StrongTypeSuffix<Meters>
{
    static constexpr const RkChar* suffix = " m";
};

template<>
struct StrongTypeSuffix<Kilometers>
{
    static constexpr const RkChar* suffix = " km";
};

// Type literals

constexpr Centimeters operator"" _cm(RkLdouble in_distance) noexcept;
constexpr Centimeters operator"" _cm(RkSize    in_distance) noexcept;
constexpr Kilometers  operator"" _km(RkLdouble in_distance) noexcept;
constexpr Kilometers  operator"" _km(RkSize    in_distance) noexcept;
constexpr Meters      operator"" _m (RkLdouble in_distance) noexcept;
constexpr Meters      operator"" _m (RkSize    in_distance) noexcept;

#include "Types/Units/Distance/Distance.inl"

END_RUKEN_NAMESPACE