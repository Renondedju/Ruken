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
#include "Maths/Math.hpp"
#include "Maths/Trigonometry.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Maths/Vector/Layouts/VectorLayout.hpp"
#include "Maths/Vector/Layouts/DistanceLayout.hpp"

#include "Types/Units/Distance/Meters.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Generic representation of x dimensional vectors and points in space
 *
 * Fundamental types are allowed as underlying types but are not recommended (such as float or int) !
 * Please use the various specializations to keep your code explicit and readable
 * Type aliases are provided: see Vector2m, Vector3px, Vector2deg ect...
 *
 * \tparam TDimensions Number of dimensions (or size) of the vector 
 * \tparam TUnderlyingType Underlying type of the vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct Vector : VectorLayout<TDimensions, TUnderlyingType>
{
    #pragma region Usings

    /**
     * \brief Underlying layout of the vector
     */
    using Layout = VectorLayout<TDimensions, TUnderlyingType>;

    /**
     * \brief Returns the largest common vector type
     * \tparam TOtherDimensions Dimensions or size of the other vector
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    using LargestVector = Vector<std::max(TDimensions, TOtherDimensions), std::common_type_t<TUnderlyingType, TOtherUnderlyingType>>;

    /**
     * \brief Returns the smallest common vector type
     * \tparam TOtherDimensions Dimensions or size of the other vector
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    using SmallestVector = Vector<std::min(TDimensions, TOtherDimensions), std::common_type_t<TUnderlyingType, TOtherUnderlyingType>>;

    /**
     * \brief Returns a common vector type for 2 vectors of the same size 
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     */
    template <typename TOtherUnderlyingType>
    using CommonSizedVector = Vector<TDimensions, std::common_type_t<TUnderlyingType, TOtherUnderlyingType>>;

    #pragma endregion

    #pragma region Utility

    /**
     * \brief Returns a reference onto the largest of the two vectors
     * If the 2 passed vectors have the same size, then in_vector will be returned
     *
     * \tparam TOtherDimensions Dimensions or size of the other vector
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Other vector reference
     * \return Reference of the largest vector
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    constexpr LargestVector<TOtherDimensions, TOtherUnderlyingType>& GetLargestVector(
        Vector<TOtherDimensions, TOtherUnderlyingType>& in_vector) const noexcept
    {
        return TDimensions >= TOtherDimensions ? *this : in_vector;
    }

    /**
     * \brief Returns a reference onto the smallest of the two vectors
     * If the 2 passed vectors have the same size, then *this will be returned
     *
     * \tparam TOtherDimensions Dimensions or size of the other vector
     * \tparam TOtherUnderlyingType Underlying type of the other vector
     * \param in_vector Other vector reference
     * \return Reference of the smallest vector
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    constexpr LargestVector<TOtherDimensions, TOtherUnderlyingType> const& GetSmallestVector(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        return TDimensions < TOtherDimensions ? *this : in_vector;
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
        TUnderlyingType square_length {};
                                                                                                                                                                                                             
        for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
            square_length += Layout::data[index] * Layout::data[index];                                                                                                                               
                                                                                                                                                                                                             
        return square_length;                                                                                                                                                                                       
    }

    /**
     * \brief Computes the length of the vector
     * \note If you only need to compare magnitudes of some vectors, you can compare squared magnitudes of
     * \note them using SquareLength instead (computing squared magnitudes is faster).
     * \return Length of the vector
     */
    [[nodiscard]]
    constexpr TUnderlyingType Length() const noexcept
    {                 
        return static_cast<TUnderlyingType>(Sqrt(static_cast<RkFloat>(SquareLength())));                                                                                                                                                                                       
    }

    /**
     * \brief Returns the nomalized vector (with a length of 1)
     * If the vector is too small to be normalized this method set the vector to 0.
     */
    [[nodiscard]]
    constexpr Vector Normalized() noexcept
    requires requires(TUnderlyingType in_a, TUnderlyingType in_b) { in_a == in_b; in_a /= in_b; }
    {
        TUnderlyingType length = Length();
        if (length == TUnderlyingType())
            return Vector {};

        Vector result {*this};
        for(RkSize index {0ULL}; index < TDimensions; ++index)                                                                                                                  
            result.data[index] /= length;

        return result;
    }

    /**
     * \brief Dot Product of two vectors
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
    constexpr std::common_type_t<TUnderlyingType, TOtherUnderlyingType> Dot(
        Vector<TDimensions, TOtherUnderlyingType> const& in_vector)
    {
        std::common_type_t<TUnderlyingType, TOtherUnderlyingType> dot {};

        for (RkSize index = 0; index < TDimensions; index++)
            dot += Layout::data[index] * in_vector.data[index];

        return dot;
    }

    /**
     * \brief Cross Product of two vectors
     *
     * The cross product of two vectors results in a third vector which is perpendicular to the two input vectors.
     * The result's magnitude is equal to the magnitudes of the two inputs multiplied together and then multiplied by the sine of the angle between the inputs.
     * You can determine the direction of the result vector using the "left hand rule"
     *
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Cross product
     */
    template<typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a * in_b - in_b * in_a; }
    [[nodiscard]] constexpr CommonSizedVector<TOtherUnderlyingType> Cross(
        Vector<TDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        CommonSizedVector<TOtherUnderlyingType> cross;

        for (RkSize index {0ULL}; index < TDimensions; ++index)
            cross.data[index] = Layout  ::data[(index + 1) % TDimensions] * in_vector.data[(index + 2) % TDimensions] -
                                in_vector.data[(index + 1) % TDimensions] * Layout  ::data[(index + 2) % TDimensions];

        return cross;
    }

    /**
     * \brief Returns a vector that is made from the largest components of two vectors
     * If one of the vectors is largest than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Max vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] constexpr LargestVector<TOtherDimensions, TOtherUnderlyingType> Max(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        SmallestVector<TOtherDimensions, TOtherUnderlyingType> const& smallest {GetSmallestVector(in_vector)};
        LargestVector <TOtherDimensions, TOtherUnderlyingType>        vector   {GetLargestVector (in_vector)};

        for(RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)
            vector.data[index] = std::max(vector.data[index], smallest.data[index]);

        return vector;
    }

    /**
     * \brief Returns a vector that is made from the smallest components of two vectors
     * If one of the vectors is largest than the other, the last(s) components will simply be reused
     *
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Right hand side vector
     * \return Min vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_a, TOtherUnderlyingType in_b) { in_a < in_b; in_a > in_b; }
    [[nodiscard]] constexpr LargestVector<TOtherDimensions, TOtherUnderlyingType> Min(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept
    {
        SmallestVector<TOtherDimensions, TOtherUnderlyingType> const& smallest {GetSmallestVector(in_vector)};
        LargestVector <TOtherDimensions, TOtherUnderlyingType>        vector   {GetLargestVector (in_vector)};

        for(RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)
            vector.data[index] = std::min(vector.data[index], smallest.data[index]);

        return vector;
    }

    /**
     * \brief Performs a linear interpolation between this vector (source) and in_vector (destination)
     * \tparam TOtherDimensions Dimensions or size of the right hand side vector
     * \tparam TOtherUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Interpolated vector
     */
    template<RkSize TOtherDimensions, typename TOtherUnderlyingType>
    [[nodiscard]] constexpr LargestVector<TOtherDimensions, TOtherUnderlyingType> Lerp(
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector,
        RkFloat                                        const  in_ratio) const noexcept
    {
        return *this + in_ratio * (in_vector - *this);
    }

    /**
     * \brief Performs a circular interpolation between this vector (source) and in_vector (destination)
     * \tparam TRhsDimensions Dimensions or size of the right hand side vector
     * \tparam TRhsUnderlyingType Underlying type of the right hand side vector
     * \param in_vector Destination vector
     * \param in_ratio Translation ratio (not clamped)
     * \return Interpolated vector
     */
    template<RkSize TRhsDimensions, typename TRhsUnderlyingType>
    [[nodiscard]] constexpr LargestVector<TRhsDimensions, TRhsUnderlyingType> Slerp(
        Vector<TRhsDimensions, TRhsUnderlyingType> const& in_vector,
        RkFloat                                    const  in_ratio)
    {
        std::common_type_t<TUnderlyingType, TRhsUnderlyingType> const dot   = Dot(in_vector);
        Radians                                                 const theta = ArcCos(dot) * in_ratio;

        return *this * Cos(theta) + (*this - in_vector * dot).Normalized() * Sin(theta);
    }

    #pragma endregion

    #pragma region Meta

    // The following macros are used to quickly implement multiple methods at once

    /**
     * \brief Implements an operator between 2 vectors
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_OPERATOR_MIXIN(in_operator) \
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>                                                    \
    requires requires (TUnderlyingType in_lhs, TOtherUnderlyingType in_rhs) { in_lhs RUKEN_GLUE(in_operator,=) in_rhs; } \
    [[nodiscard]]                                                                                                        \
    constexpr LargestVector<TOtherDimensions, TOtherUnderlyingType> RUKEN_GLUE(operator,in_operator)(                    \
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) const noexcept                                  \
    {                                                                                                                    \
        auto        largest  {GetLargestVector (in_vector)};                                                             \
        auto const& smallest {GetSmallestVector(in_vector)};                                                             \
                                                                                                                         \
        for (RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)                              \
            largest.data[index] RUKEN_GLUE(in_operator,=) smallest.data[index];                                          \
                                                                                                                         \
        return largest;                                                                                                  \
    }

    /**
     * \brief Implements an operator assignment between 2 vectors
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(in_operator) \
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>                                                    \
    requires requires (TUnderlyingType in_lhs, TOtherUnderlyingType in_rhs) { in_lhs RUKEN_GLUE(in_operator,=) in_rhs; } \
    constexpr Vector& RUKEN_GLUE(RUKEN_GLUE(operator,in_operator),=)(                                                    \
        Vector<TOtherDimensions, TOtherUnderlyingType> const& in_vector) noexcept                                        \
    {                                                                                                                    \
        for (RkSize index {0ULL}; index < std::min(TDimensions, TOtherDimensions); ++index)                              \
            Layout::data[index] RUKEN_GLUE(in_operator,=) in_vector.data[index];                                         \
                                                                                                                         \
        return *this;                                                                                                    \
    }

    /**
     * \brief Implements an operator between a vector and any scalar type
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(in_operator) \
    template <typename TScalarType>                                                                                        \
    requires requires (TUnderlyingType in_vector, TScalarType in_scalar) { in_vector in_operator in_scalar; }              \
    [[nodiscard]]                                                                                                          \
    constexpr CommonSizedVector<TScalarType> RUKEN_GLUE(operator,in_operator)(TScalarType const& in_scalar) const noexcept \
    {                                                                                                                      \
        CommonSizedVector<TScalarType> result {};                                                                          \
                                                                                                                           \
        for (RkSize index {0ULL}; index < TDimensions; ++index)                                                            \
            result.data[index] = Layout::data[index] in_operator in_scalar;                                                \
                                                                                                                           \
        return result;                                                                                                     \
    }

    /**
     * \brief Implements an assignment operator between a vector and any scalar type
     * \param in_operator Operator to implement
     */
    #define RUKEN_VECTOR_ASSIGNMENT_OPERATOR_SCALAR_MIXIN(in_operator) \
    template <typename TScalarType>                                                                                         \
    requires requires (TUnderlyingType in_vector, TScalarType in_scalar) { in_vector RUKEN_GLUE(in_operator,=) in_scalar; } \
    constexpr Vector& RUKEN_GLUE(RUKEN_GLUE(operator,in_operator),=)(TScalarType const& in_scalar) noexcept                 \
    {                                                                                                                       \
        for (RkSize index {0ULL}; index < TDimensions; ++index)                                                             \
            Layout::data[index] RUKEN_GLUE(in_operator,=) in_scalar;                                                        \
                                                                                                                            \
        return *this;                                                                                                       \
    }

    #pragma endregion

    #pragma region Operators

    RUKEN_VECTOR_OPERATOR_MIXIN(+)
    RUKEN_VECTOR_OPERATOR_MIXIN(-)
    RUKEN_VECTOR_OPERATOR_MIXIN(/)
    RUKEN_VECTOR_OPERATOR_MIXIN(*)
    RUKEN_VECTOR_OPERATOR_MIXIN(%)

    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(+)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(-)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(/)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(*)
    RUKEN_VECTOR_OPERATOR_SCALAR_MIXIN(%)

    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(+)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(-)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(/)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(*)
    RUKEN_VECTOR_ASSIGNMENT_OPERATOR_MIXIN(%)

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
    [[nodiscard]] constexpr Vector<TDimensions, TUnderlyingType> operator-()
    requires requires(TUnderlyingType in_a) { -in_a; }
    {
        Vector vector;

        for(RkSize index {0ULL}; index < TDimensions; ++index)
            vector.data[index] = -Layout::data[index];

        return vector;
    }

    /**
     * \brief Conditionally explicit conversion operator
     * \note If casting from TOtherUnderlyingType to TUnderlyingType is explicit, this method will be too, otherwise it will remain implicit
     * \tparam TOtherDimensions Dimensions to convert from
     * \tparam TOtherUnderlyingType Underlying type to convert from
     */
    template <RkSize TOtherDimensions, typename TOtherUnderlyingType>
    requires requires (TUnderlyingType in_lhs, TOtherUnderlyingType in_rhs) { in_lhs = static_cast<TUnderlyingType>(in_rhs); }
    explicit (!std::is_convertible_v<TUnderlyingType, TOtherUnderlyingType>)
    constexpr operator Vector<TOtherDimensions, TOtherUnderlyingType>() noexcept(noexcept(std::is_nothrow_convertible_v<TUnderlyingType, TOtherUnderlyingType>))
    {
        Vector<TOtherDimensions, TOtherUnderlyingType> vector {};

        for (RkSize index; index < std::min(TOtherDimensions, TDimensions); ++index)
            vector.data[index] = static_cast<TOtherUnderlyingType>(Layout::data[index]);

        return vector;
    }

    #pragma endregion
};

/**
 * \brief Two dimensional distance vector
 * \tparam TDistanceUnit Underlying distance unit of the vector
 */
template <EDistanceUnit TDistanceUnit = EDistanceUnit::Meters>
using Vector2   = Vector<2, Distance<TDistanceUnit>>;
using Vector2cm = Vector2<EDistanceUnit::Centimeters>;
using Vector2m  = Vector2<EDistanceUnit::Meters>;
using Vector2km = Vector2<EDistanceUnit::Kilometers>;

/**
 * \brief Three dimensional distance vector
 * \tparam TDistanceUnit Underlying distance unit of the vector
 */
template <EDistanceUnit TDistanceUnit = EDistanceUnit::Meters>
using Vector3   = Vector<3, Distance<TDistanceUnit>>;
using Vector3cm = Vector3<EDistanceUnit::Centimeters>;
using Vector3m  = Vector3<EDistanceUnit::Meters>;
using Vector3km = Vector3<EDistanceUnit::Kilometers>;

END_RUKEN_NAMESPACE