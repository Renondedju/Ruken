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

#include <ostream>
#include <iomanip>

#include "Build/Namespace.hpp"
#include "Maths/Trigonometry.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Generic vector class, generically implements every common vector operation
 *
 * \tparam TMemoryLayout Memory layout of the vector, this is used to define unions without having to use crtp or weird interfaces
 */
template<typename TMemoryLayout>
struct __declspec(novtable) Vector final: public TMemoryLayout
{
    using UnderlyingType = typename TMemoryLayout::UnderlyingType;

    #pragma region Constructors

    using TMemoryLayout::TMemoryLayout;

    constexpr Vector()                      = default;
    constexpr Vector(Vector const& in_copy) = default;
    constexpr Vector(Vector&&      in_move) = default;
             ~Vector()                      = default;

    #pragma endregion

    #pragma region Constants

    // Dimensions or size of the vector
    static constexpr RkSize dimensions = TMemoryLayout::dimensions;

    #pragma endregion

    #pragma region Static Methods

    /**
     * \brief Performs a linear interpolation between 2 vectors of the same type.
     *
     * \param in_from Initial vector
     * \param in_to Destination vector
     * \param in_ratio Translation ratio
     *
     * \return Interpolated vector
     */
    [[nodiscard]]
    constexpr static Vector Lerp(Vector const& in_from, Vector const& in_to, RkFloat in_ratio) noexcept;

    /**
     * \brief Performs a circular interpolation between 2 vectors of the same type.
     *
     * \param in_from Initial vector
     * \param in_to Destination vector
     * \param in_ratio Translation ratio
     *
     * \return Interpolated vector
     */
    [[nodiscard]]
    constexpr static Vector Slerp(Vector const& in_from, Vector const& in_to, RkFloat in_ratio) noexcept;

    /**
     * \brief Return the dot product value performed with 2 vectors
     *
     * \param in_lhs The first vector
     * \param in_rhs The second vector
     *
     * \return The dot product
     */
    [[nodiscard]]
    constexpr static UnderlyingType Dot(Vector const& in_lhs, Vector const& in_rhs) noexcept;

    /**
     * \brief Return the cross product value performed with 2 vectors
     *
     * \param in_lhs The first vector
     * \param in_rhs The second vector
     *
     * \return The cross product
     */
    [[nodiscard]]
    constexpr static Vector Cross(Vector const& in_lhs, Vector const& in_rhs) noexcept;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Computes the length of the vector in TType value.
     * In some cases, you might want to use SqrLength which is less expensive to compute.
     *
     * \return The length of the vector.
     */
    [[nodiscard]]
    constexpr UnderlyingType Length() const noexcept;

    /**
     * \brief Computes the squared length of the vector in TType value.
     *
     * \return The squared length of the vector.
     */
    [[nodiscard]] 
    constexpr UnderlyingType SqrLength() const noexcept;

    /**
     * \brief Normalize the vector. If the vector is null (all components are set to 0), nothing is done.
     */
    constexpr Vector& Normalize() noexcept;

    #pragma endregion

    #pragma region Operators

    constexpr Vector& operator=(Vector const& in_copy) = default;
    constexpr Vector& operator=(Vector&&      in_move) = default;

    // Boolean operators
    [[nodiscard]] constexpr RkBool operator==(Vector const& in_rhs) const noexcept;
    [[nodiscard]] constexpr RkBool operator!=(Vector const& in_rhs) const noexcept;

    // Arithmetic operators (/w vectors)
    [[nodiscard]] constexpr Vector operator+(Vector const& in_rhs) const noexcept;
    [[nodiscard]] constexpr Vector operator-(Vector const& in_rhs) const noexcept;
    [[nodiscard]] constexpr Vector operator*(Vector const& in_rhs) const noexcept;
    [[nodiscard]] constexpr Vector operator/(Vector const& in_rhs) const noexcept;

    // Arithmetic operators (/w scalars)
    [[nodiscard]] constexpr Vector operator+(UnderlyingType in_factor) const noexcept;
    [[nodiscard]] constexpr Vector operator-(UnderlyingType in_factor) const noexcept;
    [[nodiscard]] constexpr Vector operator*(UnderlyingType in_factor) const noexcept;
    [[nodiscard]] constexpr Vector operator/(UnderlyingType in_factor) const noexcept;

    [[nodiscard]] friend constexpr Vector operator*(UnderlyingType in_factor, Vector const& in_vector) noexcept { return in_vector * in_factor; }

    // Assign arithmetic operators (/w vectors)
    constexpr Vector& operator+=(Vector const& in_rhs) noexcept;
    constexpr Vector& operator-=(Vector const& in_rhs) noexcept;
    constexpr Vector& operator*=(Vector const& in_rhs) noexcept;
    constexpr Vector& operator/=(Vector const& in_rhs) noexcept;

    // Assign arithmetic operators (/w scalars)
    constexpr Vector& operator+=(UnderlyingType in_factor) noexcept;
    constexpr Vector& operator-=(UnderlyingType in_factor) noexcept;
    constexpr Vector& operator*=(UnderlyingType in_factor) noexcept;
    constexpr Vector& operator/=(UnderlyingType in_factor) noexcept;

    // Unary operators
    [[nodiscard]] constexpr Vector operator-() const noexcept;

    #pragma endregion
};

#include "Maths/Vector/New/Vector.inl"

/**
 * \brief ostream <<operator overload for the BaseVector class
 *
 * \param inout_stream Output stream 
 * \param in_vector Vector to display
 * 
 * \return Output stream instance
 */
template<typename TMemoryLayout>
std::ostream & operator<<(std::ostream & inout_stream, Vector<TMemoryLayout> const& in_vector)
{
    inout_stream << std::setprecision(3) << std::fixed;
    for (RkSize index = 0ull; index < Vector<TMemoryLayout>::dimensions; ++index)
        inout_stream << in_vector.data[index] << ' ';

    return inout_stream;
}

END_RUKEN_NAMESPACE