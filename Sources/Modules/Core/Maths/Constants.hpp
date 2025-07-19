
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Holds the constants for every passed type 
 * \tparam TType Type to find the constants for
 */
template <typename TType>
struct Constants
{};

template <>
struct Constants<RkFloat>
{
	static constexpr RkFloat e          = 2.71828182845904523536F; // e
    static constexpr RkFloat log2_e     = 1.44269504088896340736F; // log2(e)
    static constexpr RkFloat log10_e    = 0.43429448190325182765F; // log10(e)
    static constexpr RkFloat ln2        = 0.69314718055994530942F; // ln(2)
    static constexpr RkFloat ln10       = 2.30258509299404568402F; // ln(10)
    static constexpr RkFloat pi         = 3.14159265358979323846F; // pi
    static constexpr RkFloat pi_2       = 1.57079632679489661923F; // pi/2
    static constexpr RkFloat pi_4       = 0.78539816339744830962F; // pi/4
    static constexpr RkFloat _1_pi      = 0.31830988618379067154F; // 1/pi
    static constexpr RkFloat _2_pi      = 0.63661977236758134308F; // 2/pi
    static constexpr RkFloat _2_sqrt_pi = 1.12837916709551257390F; // 2/sqrt(pi)
    static constexpr RkFloat sqrt_2     = 1.41421356237309504880F; // sqrt(2)
    static constexpr RkFloat _1_sqrt_2  = 0.70710678118654752440F; // 1/sqrt(2)
};

END_RUKEN_NAMESPACE