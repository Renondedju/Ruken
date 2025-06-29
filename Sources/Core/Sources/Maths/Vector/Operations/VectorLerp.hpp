
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
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    /**
     * \brief Performs a linear interpolation between this vector (source) and in_vector (destination)
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     *
     * \param in_source Source vector
     * \param in_destination Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Interpolated vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    [[nodiscard]]
    static constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Lerp(
        Vector<TDimensions, TUnderlyingType>           const& in_source,
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_destination,
        RkFloat                                        const  in_ratio) noexcept
    {
        return in_source + (in_destination - in_source) * in_ratio;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Performs a linear interpolation between this vector (source) and in_vector (destination)
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     *
     * \param in_destination Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Interpolated vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    [[nodiscard]]
    constexpr auto Lerp(Vector<TOtherDimensions, TOtherUnderlyingType> const& in_destination, RkFloat const in_ratio) const noexcept
    {
        return Lerp(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_destination, in_ratio);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE