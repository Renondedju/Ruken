#pragma once

#include "Maths/Trigonometry.hpp"
#include "Maths/Vector/Operations/VectorDot.hpp"
#include "Maths/Vector/Operations/VectorNormalization.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Implements vector Slerp method
 *
 * @tparam TDimensions Dimensions or size of the composed vector
 * @tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorSlerp
{
    #pragma region Static Methods

    /**
     * @brief Performs a circular interpolation between this vector (source) and in_vector (destination)
     *
     * @param in_source Source vector
     * @param in_destination Destination vector
     * @param in_ratio Translation ratio (not clamped)
     * @return Circularly interpolated vector
     */
    [[nodiscard]]
    static constexpr Vector<TDimensions, TUnderlyingType> Slerp(
        Vector<TDimensions, TUnderlyingType> const& in_source,
        Vector<TDimensions, TUnderlyingType> const& in_destination,
        RkFloat                              const  in_ratio) noexcept
    {
        RkFloat const dot   = VectorDot<TDimensions, TUnderlyingType>::Dot(in_source, in_destination);
        Radians const theta = ArcCos(dot) * in_ratio;

        return in_source * Cos(theta) + VectorNormalization<TDimensions, TUnderlyingType>::Normalized(in_source - in_destination * dot) * Sin(theta);
    }

    #pragma endregion

    #pragma region Methods

    /**
     * @brief Performs a circular interpolation between this vector (source) and in_vector (destination)
     * @param in_destination Destination vector
     * @param in_ratio Translation ratio (not clamped)
     * @return Circularly interpolated vector
     */
    [[nodiscard]]
    constexpr auto Slerp(Vector<TDimensions, TUnderlyingType> const& in_destination, RkFloat const in_ratio) const noexcept
    {
        return Slerp(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_destination, in_ratio);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE