
#pragma once

#include "Meta/Meta.hpp"
#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements every common vector operator
 * Implements every common vector operator (*, -, +, / and %) between 2 vectors, a vector and a scalar type
 * as well as their corresponding assignment operators.
 * This class implements as well a multi-way conversion operator. 
 *
 * \tparam TDimensions Dimensions or size of the composed vector
 * \tparam TUnderlyingType Underlying type of the composed vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorOperators
{
    using TVector = Vector<TDimensions, TUnderlyingType>;
    using Helper  = VectorHelper<TDimensions, TUnderlyingType>;

    #pragma region Meta

    /**
     * \brief Implements an operator between 2 vectors
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_OPERATOR_MIXIN(in_operator) \
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>                                                    \
    requires requires (TUnderlyingType in_lhs, TOtherUnderlyingType in_rhs) { in_lhs RUKEN_GLUE(in_operator,=) in_rhs; } \
    [[nodiscard]]                                                                                                        \
    constexpr typename Helper::template LargestVector<TOtherDimensions, TOtherUnderlyingType> RUKEN_GLUE(operator,in_operator)( \
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept                                  \
    {                                                                                                                    \
        if constexpr (TDimensions >= TOtherDimensions)                                                                   \
        {                                                                                                                \
            auto vector {*static_cast<TVector const*>(this)};                                                            \
                                                                                                                         \
            for (RkSize index {0ULL}; index < TOtherDimensions; ++index)                                                 \
                vector.data[index] RUKEN_GLUE(in_operator,=) in_vector.data[index];                                      \
                                                                                                                         \
            return vector;                                                                                               \
        }                                                                                                                \
                                                                                                                         \
        auto vector {in_vector};                                                                                         \
                                                                                                                         \
        for (RkSize index {0ULL}; index < TDimensions; ++index)                                                          \
            vector.data[index] RUKEN_GLUE(in_operator,=) static_cast<TVector const*>(this)->data[index];                 \
                                                                                                                         \
        return vector;                                                                                                   \
    }

    /**
     * \brief Implements an operator assignment between 2 vectors
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(in_operator) \
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>                                                    \
    requires requires (TUnderlyingType in_lhs, TOtherUnderlyingType in_rhs) { in_lhs RUKEN_GLUE(in_operator,=) in_rhs; } \
    constexpr TVector& RUKEN_GLUE(RUKEN_GLUE(operator,in_operator),=)(                                                   \
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) noexcept                                        \
    {                                                                                                                    \
        for (RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)                              \
            static_cast<TVector*>(this)->data[index] RUKEN_GLUE(in_operator,=) in_vector.data[index];                    \
                                                                                                                         \
        return *static_cast<TVector*>(this);                                                                             \
    }

    /**
     * \brief Implements an operator between a vector and any scalar type
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(in_operator) \
    template <typename TScalarType>                                                                           \
    requires requires (TUnderlyingType in_vector, TScalarType in_scalar) { in_vector in_operator in_scalar; } \
    [[nodiscard]]                                                                                             \
    constexpr typename Helper::template CommonSizedVector<TScalarType> RUKEN_GLUE(operator,in_operator)(      \
        TScalarType const& in_scalar) const noexcept                                                          \
    {                                                                                                         \
        using Type = CommonTypeFallback<TUnderlyingType, TUnderlyingType, TScalarType>;                       \
        typename Helper::template CommonSizedVector<TScalarType> result {};                                   \
                                                                                                              \
        for (RkSize index {0ULL}; index < TDimensions; ++index)                                               \
            result.data[index] = static_cast<Type>(static_cast<TVector const*>(this)->data[index]                               \
                in_operator static_cast<Type>(in_scalar));                                                     \
                                                                                                              \
        return result;                                                                                        \
    }

    /**
     * \brief Implements an assignment operator between a vector and any scalar type
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(in_operator) \
    template <typename TScalarType>                                                                                         \
    requires requires (TUnderlyingType in_vector, TScalarType in_scalar) { in_vector RUKEN_GLUE(in_operator,=) in_scalar; } \
    constexpr TVector& RUKEN_GLUE(RUKEN_GLUE(operator,in_operator),=)(TScalarType const& in_scalar) noexcept                \
    {                                                                                                                       \
        for (RkSize index {0ULL}; index < TDimensions; ++index)                                                             \
            static_cast<TVector*>(this)->data[index] RUKEN_GLUE(in_operator,=) static_cast<TUnderlyingType>(in_scalar);     \
                                                                                                                            \
        return *static_cast<TVector*>(this);                                                                                \
    }

    #pragma endregion

    #pragma region Operators

    #pragma warning( push )
    #pragma warning( disable:4702 ) // Unreachable code (due to if constexpr)

    // Vector - Vector operators

    RUKEN_VECTOR_OPERATOR_MIXIN(+)
    RUKEN_VECTOR_OPERATOR_MIXIN(-)
    RUKEN_VECTOR_OPERATOR_MIXIN(/)
    RUKEN_VECTOR_OPERATOR_MIXIN(*)
    RUKEN_VECTOR_OPERATOR_MIXIN(%)

    #pragma warning( pop )

    // Vector - Scalar operators

    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(+)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(-)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(/)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(*)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(%)

    // Vector - Vector assignment operators

    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(+)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(-)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(/)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(*)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(%)

    // Vector - Scalar assignment operators

    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(+)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(-)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(/)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(*)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(%)

    #undef RUKEN_VECTOR_OPERATOR_MIXIN
    #undef RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN
    #undef RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN
    #undef RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN

    /**
     * \brief Inverts the content of the vector (unary -)
     * \return Inverted vector
     */
    [[nodiscard]]
    constexpr TVector operator-() const noexcept
    requires requires(TUnderlyingType in_a) { -in_a; }
    {
        TVector vector;

        for(RkSize index {0ULL}; index < TDimensions; ++index)
            vector.data[index] = -static_cast<TVector*>(this)->data[index];

        return vector;
    }

    /**
     * \brief Right shift ostream operator
     * \param in_stream Output stream to write to
     * \param in_vector Vector instance to write
     * \return Output stream reference
     */
    friend std::ostream& operator<<(std::ostream& in_stream, TVector const& in_vector) noexcept
    {
        in_stream << '(';
        for (RkSize index {0ULL}; index < TDimensions; ++index)
            if (index < TDimensions - 1)
                in_stream << in_vector.data[index] << ", ";
            else
                in_stream << in_vector.data[index];

        return in_stream << ')';
    }

    /**
     * \brief Conditionally explicit conversion operator
     * \note If casting from TOtherUnderlyingType to TUnderlyingType is explicit, this method will be too, otherwise it will remain implicit
     * \tparam TOtherDimensions Dimensions to convert from
     * \tparam TOtherUnderlyingType Underlying type to convert from
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires   std::is_convertible_v<TOtherUnderlyingType, TUnderlyingType>
    explicit (!std::is_convertible_v<TUnderlyingType, TOtherUnderlyingType>)
    [[nodiscard]]
    constexpr operator Vector<TOtherDimensions, TOtherUnderlyingType>() const noexcept(
        noexcept(std::is_nothrow_convertible_v<TUnderlyingType, TOtherUnderlyingType>))
    {
        Vector<TOtherDimensions, TOtherUnderlyingType> vector {};

        for (RkSize index {0ULL}; index < std::min(TOtherDimensions, TDimensions); ++index)
            vector.data[index] = static_cast<TOtherUnderlyingType>(static_cast<TVector const*>(this)->data[index]);

        return vector;
    }

    #pragma endregion
};

END_RUKEN_NAMESPACE