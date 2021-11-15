
#pragma once

#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Cross method
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorCross
{
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    /**
     * \brief Cross Product of two vectors
     *
     * The cross product of two vectors results in a third vector which is perpendicular to the two input vectors.
     * The result's magnitude is equal to the magnitudes of the two inputs multiplied together and then multiplied by the sine of the angle between the inputs.
     * You can determine the direction of the result vector using the "left hand rule"
     *
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Left hand side vector
     * \param in_other_vector Right hand side vector
     * \return Cross product
     */
    template<typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a * in_b - in_b * in_a; }
    [[nodiscard]] static constexpr typename Helper::template CommonSizedVector<TOtherUnderlyingType> Cross(
        Vector<TDimensions, TUnderlyingType>      const& in_vector,
        Vector<TDimensions, TOtherUnderlyingType> const& in_other_vector) noexcept
    {
        typename Helper::template CommonSizedVector<TOtherUnderlyingType> cross;

        for (RkSize index {0ULL}; index < TDimensions; ++index)
            cross.data[index] = in_vector      .data[(index + 1) % TDimensions] * in_other_vector.data[(index + 2) % TDimensions] -
                                in_other_vector.data[(index + 1) % TDimensions] * in_vector      .data[(index + 2) % TDimensions];

        return cross;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Cross Product of two vectors
     *
     * The cross product of two vectors results in a third vector which is perpendicular to the two input vectors.
     * The result's magnitude is equal to the magnitudes of the two inputs multiplied together and then multiplied by the sine of the angle between the inputs.
     * You can determine the direction of the result vector using the "left hand rule"
     *
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_other_vector Right hand side vector
     * \return Cross product
     */
    template<typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a * in_b - in_b * in_a; }
    [[nodiscard]] constexpr auto Cross(Vector<TDimensions, TOtherUnderlyingType> const& in_other_vector) const noexcept
    {
        return Cross(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_other_vector);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE