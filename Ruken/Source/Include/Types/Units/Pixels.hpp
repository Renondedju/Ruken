
#pragma once

#include "Build/Attributes/EmptyBases.hpp"
#include "Types/StrongType/StrongType.hpp"
#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"
#include "Types/StrongType/Operators/Stream.hpp"

BEGIN_RUKEN_NAMESPACE

struct RUKEN_EMPTY_BASES Pixels final:
    StrongType<RkInt32, Pixels>,
    Arithmetic<Pixels>,
    Comparison<Pixels>,
    Stream    <Pixels>
{
    using StrongType<RkInt32, Pixels>::StrongType;
};

// Suffixes

template<>
struct StrongTypeSuffix<Pixels>
{
    static constexpr const RkChar* suffix = " px";
};

constexpr Pixels operator"" _px(RkSize const in_pixels) noexcept
{
    return Pixels(static_cast<RkInt32>(in_pixels));
}

#include "Types/Units/Pixels.hpp"

END_RUKEN_NAMESPACE