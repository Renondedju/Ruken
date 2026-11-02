
#pragma once

#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Lerp method
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorLerp
{
    #pragma region Static Methods

    /**
     * @brief Performs a linear interpolation between this vector (source) and in_vector (destination)
     *
     * @param in_source Source vector
     * @param in_destination Destination vector
     * @param in_ratio Translation ratio (not clamped)
     * @return Interpolated vector
     */
    [[nodiscard]]
    static constexpr Vector<TDimensions, TUnderlyingType> Lerp(
        Vector<TDimensions, TUnderlyingType> const& in_source,
        Vector<TDimensions, TUnderlyingType> const& in_destination,
        RkFloat                              const  in_ratio) noexcept
    {
        return in_source + (in_destination - in_source) * in_ratio;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * @brief Performs a linear interpolation between this vector (source) and in_vector (destination)
     *
     * @param in_destination Destination vector
     * @param in_ratio Translation ratio (not clamped)
     * @return Interpolated vector
     */
    [[nodiscard]]
    constexpr auto Lerp(Vector<TDimensions, TUnderlyingType> const& in_destination, RkFloat const in_ratio) const noexcept
    {
        return Lerp(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_destination, in_ratio);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE