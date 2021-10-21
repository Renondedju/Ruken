/**
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include "Meta/Meta.hpp"
#include "Maths/Vector/Helper/VectorHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \tparam TVector Composed vector type, must inherit this class and be an instance of the Vector class
 */
template <typename TVector>
struct VectorOperators;

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
struct VectorOperators<Vector<TDimensions, TUnderlyingType>>
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
        typename Helper::template CommonSizedVector<TScalarType> result {};                                   \
                                                                                                              \
        for (RkSize index {0ULL}; index < TDimensions; ++index)                                               \
            result.data[index] = static_cast<TVector const*>(this)->data[index] in_operator in_scalar;        \
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
            static_cast<TVector*>(this)->data[index] RUKEN_GLUE(in_operator,=) in_scalar;                                   \
                                                                                                                            \
        return *static_cast<TVector*>(this);                                                                                \
    }

    #pragma endregion

    #pragma region Operators

    // Vector - Vector operators

    RUKEN_VECTOR_OPERATOR_MIXIN(+)
    RUKEN_VECTOR_OPERATOR_MIXIN(-)
    RUKEN_VECTOR_OPERATOR_MIXIN(/)
    RUKEN_VECTOR_OPERATOR_MIXIN(*)
    RUKEN_VECTOR_OPERATOR_MIXIN(%)

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