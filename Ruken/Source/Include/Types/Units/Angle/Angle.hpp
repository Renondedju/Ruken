
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Maths/Constants.hpp"
#include "Types/Units/Angle/EAngleUnit.hpp"
#include "Types/StrongType/StrongType.hpp"
#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"
#include "Types/StrongType/Operators/Stream.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed angle type
 * \tparam TUnitType Unit type of the angle, must be either Degrees or Radians
 */
template<EAngleUnit TUnitType>
struct RUKEN_EMPTY_BASES Angle final:
    StrongType<RkFloat, Angle<TUnitType>>,
    Arithmetic<Angle<TUnitType>>,
    Comparison<Angle<TUnitType>>,
    Stream    <Angle<TUnitType>>
{
    #pragma region Constructors

    using StrongType<RkFloat, Angle<TUnitType>>::StrongType;

    constexpr Angle()                     = default;
    constexpr Angle(Angle const& in_copy) = default;
    constexpr Angle(Angle&&      in_move) = default;
             ~Angle()                     = default;

    #pragma endregion

    #pragma region Operators

    constexpr Angle& operator=(Angle const& in_copy) = default;
    constexpr Angle& operator=(Angle&&		in_move) = default;

    /**
     * \brief Degree angle conversion
     */
    [[nodiscard]]
    constexpr operator Angle<EAngleUnit::Degree>() const noexcept;

    /**
     * \brief Radian angle conversion
     */
    [[nodiscard]]
    constexpr operator Angle<EAngleUnit::Radian>() const noexcept;

    #pragma endregion
};

// Type aliases

using Degrees = Angle<EAngleUnit::Degree>;
using Radians = Angle<EAngleUnit::Radian>;

// Suffixes

template<>
struct StrongTypeSuffix<Degrees>
{
    static constexpr const RkChar* suffix = " deg";
};

template<>
struct StrongTypeSuffix<Radians>
{
    static constexpr const RkChar* suffix = " rad";
};

// Type literals

constexpr Degrees operator"" _deg(RkLdouble in_angle) noexcept;
constexpr Degrees operator"" _deg(RkSize    in_angle) noexcept;
constexpr Radians operator"" _rad(RkLdouble in_angle) noexcept;
constexpr Radians operator"" _rad(RkSize    in_angle) noexcept;

#include "Types/Units/Angle/Angle.inl"


// Constants

template <>
struct Constants<Radians>
{
    static constexpr Radians pi   = 3.14159265358979323846_rad;
    static constexpr Radians pi_2 = 1.57079632679489661923_rad; // pi / 2
    static constexpr Radians pi_4 = 0.78539816339744830962_rad; // pi / 4
};

END_RUKEN_NAMESPACE