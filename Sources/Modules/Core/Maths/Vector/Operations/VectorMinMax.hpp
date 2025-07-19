
#pragma once

#include "Meta/IsInstance.hpp"
#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements vector Min and Max methods
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorMinMax
{
    using TVector = Vector<TDimensions, TUnderlyingType>;
    using Helper  = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Static Methods

    #pragma warning( push )
    #pragma warning( disable:4702 ) // Unreachable code (due to if constexpr)

    /**
     * \brief Returns a vector that is made from the largest components of two vectors
     *
     * If one of the vectors is larger than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Left hand side vector
     * \param in_other_vector Right hand side vector
     * \return Max vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] static constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Max(
        Vector<TDimensions, TUnderlyingType>           const& in_vector,
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_other_vector) noexcept
    {
        if constexpr (TDimensions >= TOtherDimensions)
        {
            auto vector {in_vector};

            for(RkSize index {0ULL}; index < TOtherDimensions; ++index)
                vector.data[index] = std::max(vector.data[index], in_other_vector.data[index]);

            return vector;
        }

        auto vector {in_other_vector};

        for(RkSize index {0ULL}; index < TDimensions; ++index)
            vector.data[index] = std::max(vector.data[index], in_vector.data[index]);

        return vector;
    }

    /**
     * \brief Returns a vector that is made from the smallest components of two vectors
     *
     * If one of the vectors is larger than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Left hand side vector
     * \param in_other_vector Right hand side vector
     * \return Min vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] static constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> Min(
        Vector<TDimensions, TUnderlyingType>           const& in_vector,
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_other_vector) noexcept
    {
        if constexpr (TDimensions >= TOtherDimensions)
        {
            auto vector {in_vector};

            for(RkSize index {0ULL}; index < TOtherDimensions; ++index)
                vector.data[index] = std::min(vector.data[index], in_other_vector.data[index]);

            return vector;
        }

        auto vector {in_other_vector};

        for(RkSize index {0ULL}; index < TDimensions; ++index)
            vector.data[index] = std::min(vector.data[index], in_vector.data[index]);

        return vector;
    }

    #pragma warning( pop )

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Returns a vector that is made from the largest components of two vectors
     * If one of the vectors is larger than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Max vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] auto Max(Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        return Max(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_vector);
    }

    /**
     * \brief Returns a vector that is made from the smallest components of two vectors
     * If one of the vectors is larger than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Min vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] constexpr auto Min(Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        return Min(*static_cast<Vector<TDimensions, TUnderlyingType> const*>(this), in_vector);
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE