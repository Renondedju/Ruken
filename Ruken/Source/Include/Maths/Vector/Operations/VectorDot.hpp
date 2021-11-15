
#pragma once

#include "Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Dot method
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorDot
{
    #pragma region Static Methods

    /**
     * \brief Dot Product of two vectors
     *
     * The dot product is a float value equal to the magnitudes of the
     * two vectors multiplied together and then multiplied by the cosine of the angle between them.
     *
     * \note For normalized vectors Dot returns 1 if they point in exactly the same direction,
     * \note -1 if they point in completely opposite directions and zero if the vectors are perpendicular.
     *
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Left hand side vector
     * \param in_other_vector Right hand side vector
     * \return Dot product
     */
    template<typename TOtherUnderlyingType>
    [[nodiscard]]
    static constexpr RkFloat Dot(Vector<TDimensions, TUnderlyingType>      const& in_vector,
                                 Vector<TDimensions, TOtherUnderlyingType> const& in_other_vector) noexcept
    {
        RkFloat dot {0.0F};

        for (RkSize index = 0; index < TDimensions; index++)
            dot += static_cast<RkFloat>(in_vector.data[index] * in_other_vector.data[index]);

        return dot;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Dot Product of two vectors
     *
     * The dot product is a float value equal to the magnitudes of the
     * two vectors multiplied together and then multiplied by the cosine of the angle between them.
     *
     * \note For normalized vectors Dot returns 1 if they point in exactly the same direction,
     * \note -1 if they point in completely opposite directions and zero if the vectors are perpendicular.
     *
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Right hand side vector
     * \return Dot product
     */
    template<typename TOtherUnderlyingType>
    [[nodiscard]]
    constexpr RkFloat Dot(Vector<TDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        return Dot(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_vector);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE