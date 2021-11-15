
#pragma once

#include "Maths/Vector/Helper/VectorHelper.hpp"
#include "Maths/Vector/Operations/VectorLength.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Normalize and Normalized methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorNormalization
{
    using Helper = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    /**
     * \brief Returns the normalized vector (with a length of 1)
     * If the vector is too small to be normalized a vector zero will be returned instead.
     *
     * \param in_vector Vector instance
     * \returns Normalized version of the vector 
     */
    [[nodiscard]]
    static constexpr Vector<TDimensions, TUnderlyingType> Normalized(Vector<TDimensions, TUnderlyingType> const& in_vector) noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        TUnderlyingType length = VectorLength<TDimensions, TUnderlyingType>::Length(in_vector);
        if (length == TUnderlyingType())
            return {};

        Vector<TDimensions, TUnderlyingType> result {in_vector};
        for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
            result.data[index] /= length;

        return result;
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Returns the normalized vector (with a length of 1)
     * If the vector is too small to be normalized a vector zero will be returned instead.
     * \returns Normalized version of the vector 
     */
    [[nodiscard]]
    constexpr auto Normalized() const noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        return Normalized(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    /**
     * \brief Normalizes the vector (with a length of 1)
     * If the vector is too small to be normalized this method set the vector to 0.
     */
    constexpr Vector<TDimensions, TUnderlyingType>& Normalize() noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        return *static_cast<Vector<TDimensions, TUnderlyingType>*>(this) = Normalized(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE