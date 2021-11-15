
#pragma once

#include "Maths/Math.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Length and SqrLength methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorLength
{
    #pragma region Static Methods

    /**
     * \brief Computes the square length of the vector
     * \param in_vector Vector instance
     * \return Square length of the vector
     */
    [[nodiscard]]
    static constexpr TUnderlyingType SquareLength(Vector<TDimensions, TUnderlyingType> const& in_vector) noexcept
    requires requires(TUnderlyingType in_a) { in_a += in_a; in_a * in_a; }
    {                 
        TUnderlyingType square_length {};

        for(RkSize index {0ULL}; index < TDimensions; ++index)
            square_length += in_vector.data[index] * in_vector.data[index];

        return square_length;
    }

    /**
     * \brief Computes the length of the vector
     *
     * \note If you only need to compare magnitudes of some vectors, you can compare squared magnitudes of
     * \note them using SquareLength instead (computing squared magnitude is faster).
     *
     * \param in_vector Vector instance
     * \return Length of the vector
     */
    [[nodiscard]]
    static constexpr TUnderlyingType Length(Vector<TDimensions, TUnderlyingType> const& in_vector) noexcept
    {                 
        return static_cast<TUnderlyingType>(Sqrt(static_cast<RkFloat>(SquareLength(in_vector))));
    }

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Computes the square length of the vector
     * \return Square length of the vector
     */
    [[nodiscard]]
    constexpr TUnderlyingType SquareLength() const noexcept
    requires requires(TUnderlyingType in_a) { in_a += in_a; in_a * in_a; }
    {                 
        return SquareLength(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    /**
     * \brief Computes the length of the vector
     * \note If you only need to compare magnitudes of some vectors, you can compare squared magnitudes of
     * \note them using SquareLength instead (computing squared magnitude is faster).
     * \return Length of the vector
     */
    [[nodiscard]]
    constexpr TUnderlyingType Length() const noexcept
    {                 
        return SquareLength(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this));
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE