#pragma once

#include "Build/Attributes.hpp"
#include "Types/StrongType/StrongType.hpp"
#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/Operators/Arithmetic.hpp"
#include "Types/StrongType/Operators/Comparison.hpp"
#include "Types/StrongType/Operators/Stream.hpp"

BEGIN_RUKEN_NAMESPACE

struct RUKEN_EMPTY_BASES Pixels final:
    StrongType<RkInt, Pixels>,
    Arithmetic       <Pixels>,
    Comparison,
    Stream
{
    using StrongType::StrongType;
};

// Suffixes

template<>
struct StrongTypeSuffix<Pixels>
{
    static constexpr const RkChar* suffix = " px";
};

constexpr Pixels operator""_px(RkULLInt const in_pixels) noexcept
{
    return Pixels(static_cast<RkInt32>(in_pixels));
}

#include "Types/Units/Pixels.hpp"

END_RUKEN_NAMESPACE