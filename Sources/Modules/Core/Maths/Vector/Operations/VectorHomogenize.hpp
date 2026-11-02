#pragma once

#include "Maths/Vector/Helper/VectorHelper.hpp"
#include "Maths/Vector/Operations/VectorLength.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Implements vector Homogenized method
 * @tparam TDimensions Dimensions or size of the composed vector
 * @tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorHomogenize
{
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    /**
     * Perspective divide.
     * @param in_vector Vector to homogenize.
     * @return Homogenized vector.
     */
    [[nodiscard]]
    static constexpr Vector<TDimensions - 1, TUnderlyingType> Homogenized(Vector<TDimensions, TUnderlyingType> const& in_vector) noexcept
        requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a / in_b; }
    {
        Vector<TDimensions - 1, TUnderlyingType> result {in_vector};
        for(RkSize index {0ULL}; index < TDimensions - 1; ++index)
            result.data[index] = in_vector.data[index] / in_vector.data[TDimensions - 1];

        return result;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * Perspective divide.
     * @return Homogenized vector.
     */
    [[nodiscard]]
    constexpr auto Homogenized() const noexcept
        requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a / in_b; }
    {
        return Homogenized(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE