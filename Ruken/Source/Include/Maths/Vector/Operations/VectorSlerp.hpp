
#pragma once

#include "Maths/Trigonometry.hpp"
#include "Maths/Vector/Helper/VectorHelper.hpp"
#include "Maths/Vector/Operations/VectorDot.hpp"
#include "Maths/Vector/Operations/VectorNormalization.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Slerp method
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorSlerp
{
    using TVector = Vector<TDimensions, TUnderlyingType>;
    using Helper  = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    /**
     * \brief Performs a circular interpolation between this vector (source) and in_vector (destination)
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_source Source vector
     * \param in_destination Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Circularly interpolated vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    [[nodiscard]]
    static constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Slerp(
        Vector<TDimensions, TUnderlyingType>           const& in_source,
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_destination,
        RkFloat                                        const  in_ratio) noexcept
    {
        RkFloat const dot   = VectorDot<TVector>::Dot(in_source, in_destination);
        Radians const theta = static_cast<Radians>(ArcCos(static_cast<Radians>(dot)) * in_ratio);

        return in_source * Cos(theta) + VectorNormalization<TVector>::Normalized(in_source - in_destination * dot) * Sin(theta);
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Performs a circular interpolation between this vector (source) and in_vector (destination)
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_destination Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Circularly interpolated vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    [[nodiscard]]
    constexpr auto Slerp(Vector<TOtherDimensions, TOtherUnderlyingType> const& in_destination, RkFloat const in_ratio) const noexcept
    {
        return Slerp(*static_cast<TVector const*>(this), in_destination, in_ratio);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE