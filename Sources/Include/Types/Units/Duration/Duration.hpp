
#pragma once

#include "Build/Attributes/EmptyBases.hpp"
#include "Types/StrongType/StrongType.hpp"
#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"
#include "Types/StrongType/Operators/Stream.hpp"

#include "Types/Units/Duration/EDurationUnit.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Strongly typed angle type
 * \tparam TUnitType Unit type of the angle, must be either Degrees or Radians
 */
template<EDurationUnit TUnitType>
struct RUKEN_EMPTY_BASES Duration final:
    StrongType<RkFloat, Duration<TUnitType>>,
    Arithmetic<Duration<TUnitType>>,
    Comparison<Duration<TUnitType>>,
    Stream    <Duration<TUnitType>>
{
    #pragma region Constructors

    using StrongType<RkFloat, Duration<TUnitType>>::StrongType;

    constexpr Duration()                        = default;
    constexpr Duration(Duration const& in_copy) = default;
    constexpr Duration(Duration&&      in_move) = default;
             ~Duration()                        = default;

    #pragma endregion

    #pragma region Operators

    constexpr Duration& operator=(Duration const& in_copy) = default;
    constexpr Duration& operator=(Duration&&      in_move) = default;

    /**
     * \brief Minute duration conversion
     */
    [[nodiscard]]
    constexpr operator Duration<EDurationUnit::Minute>() const noexcept;

    /**
     * \brief Second duration conversion
     */
    [[nodiscard]]
    constexpr operator Duration<EDurationUnit::Second>() const noexcept;

    /**
     * \brief Millisecond duration conversion
     */
    [[nodiscard]]
    constexpr operator Duration<EDurationUnit::Millisecond>() const noexcept;

    #pragma endregion
};

// Type aliases

using Milliseconds = Duration<EDurationUnit::Millisecond>;
using Seconds      = Duration<EDurationUnit::Second>;
using Minutes      = Duration<EDurationUnit::Minute>;

// Suffixes

template<>
struct StrongTypeSuffix<Milliseconds>
{
    static constexpr const RkChar* suffix = " ms";
};

template<>
struct StrongTypeSuffix<Seconds>
{
    static constexpr const RkChar* suffix = " sec";
};

template<>
struct StrongTypeSuffix<Minutes>
{
    static constexpr const RkChar* suffix = " min";
};


// Type literals

constexpr Milliseconds operator"" _ms (RkLdouble in_duration) noexcept;
constexpr Milliseconds operator"" _ms (RkSize    in_duration) noexcept;
constexpr Seconds      operator"" _sec(RkLdouble in_duration) noexcept;
constexpr Seconds      operator"" _sec(RkSize    in_duration) noexcept;
constexpr Minutes      operator"" _min(RkLdouble in_duration) noexcept;
constexpr Minutes      operator"" _min(RkSize    in_duration) noexcept;

#include "Types/Units/Duration/Duration.inl"

END_RUKEN_NAMESPACE