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
#include "Types/FundamentalTypes.hpp"
#include "Types/Concepts/ArithmeticType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Generic vector class, this class is meant to be inherited from. See Vector2/3/4 as examples
 *
 * Implements all the base vector operations via crtp
 *
 * \tparam TDerived Derived class, used for crtp 
 * \tparam TLength Length of the vector
 * \tparam TType Underlying type of the vector
 */
template <typename TDerived, RkSize TLength, ArithmeticType TType = RkFloat>
struct __declspec(novtable) BaseVector
{
    #pragma region Constructors

    constexpr BaseVector()                          = default;
    constexpr BaseVector(BaseVector const& in_copy) = default;
    constexpr BaseVector(BaseVector&&      in_move) = default;
             ~BaseVector()                          = default;

    #pragma endregion

    #pragma region Methods

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
    constexpr static BaseVector Lerp(BaseVector const& in_from, BaseVector const& in_to, float in_ratio) noexcept;

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
    constexpr static BaseVector Slerp(BaseVector const& in_from, BaseVector const& in_to, float in_ratio) noexcept;

    /**
     * \brief Return the dot product value performed with 2 vectors
     *
     * \param in_lhs The first vector
     * \param in_rhs The second vector
     *
     * \return The dot product
     */
    [[nodiscard]]
    constexpr static TType Dot(BaseVector const& in_lhs, BaseVector const& in_rhs) noexcept;

    /**
     * \brief Return the cross product value performed with 2 vectors
     *
     * \param in_lhs The first vector
     * \param in_rhs The second vector
     *
     * \return The cross product
     */
    [[nodiscard]]
    constexpr static BaseVector Cross(BaseVector const& in_lhs, BaseVector const& in_rhs) noexcept;

    /**
     * \brief Data getter
     *
     * \param in_index Index of the data.
     *
     * \return The data at the given index.
     */
    [[nodiscard]]
    constexpr TType const& At(size_t in_index) const noexcept;

    /**
     * \brief Data getter/setter
     *
     * \param in_index Index of the data.
     *
     * \return The data at the given index.
     */
    [[nodiscard]]
    constexpr TType& At(size_t in_index) noexcept;

    /**
     * \brief Computes the length of the vector in TType value.
     * In some cases, you might want to use SqrLength which is less expensive to compute.
     *
     * \return The length of the vector.
     */
    [[nodiscard]]
    constexpr TType Length() const noexcept;

    /**
     * \brief Computes the squared length of the vector in TType value.
     *
     * \return The squared length of the vector.
     */
    [[nodiscard]] 
    constexpr TType SqrLength() const noexcept;

    /**
     * \brief Normalize the vector. If the vector is null (all components are set to 0), nothing is done.
     */
    constexpr BaseVector& Normalize() noexcept;

    /**
     * \brief Returns the normalized vector. If the vector is null (all components are set to 0), then vector zero is returned.
     *
     * \return The normalized vector.
     */
    [[nodiscard]]
    constexpr BaseVector GetNormalized() const noexcept;

    /**
     * \brief Adds the current vector with another.
     *
     * \param in_other The other vector to add with.
     *
     * \return The result of the addition.
     */
    [[nodiscard]]
    constexpr BaseVector& Add(BaseVector const& in_other) noexcept;

    /**
     * \brief Subtracts the current vector with another.
     *
     * \param in_other The other vector to subtract with.
     *
     * \return The result of the subtraction.
     */
    [[nodiscard]]
    constexpr BaseVector& Subtract(BaseVector const& in_other) noexcept;

    /**
     * \brief Multiplies the current vector with another.
     *
     * \param in_other The other vector to multiply with.
     *
     * \return The result of the multiplication.
     */
    [[nodiscard]]
    constexpr BaseVector& Multiply(BaseVector const& in_other) noexcept;

    /**
     * \brief Divides the current vector with another.
     * If a component of the given vector is equal to 0, the current vector equivalent component is set to 0.
     *
     * \param in_other The other vector to divide with.
     *
     * \return The result of the division.
     */
    [[nodiscard]]
    constexpr BaseVector& Divide(BaseVector const& in_other) noexcept;

    /**
     * \brief Adds the current vector with another.
     *
     * \param in_value The other vector to add with.
     *
     * \return The result of the addition.
     */
    [[nodiscard]]
    constexpr BaseVector& Add(TType in_value) noexcept;

    /**
     * \brief Subtracts the current vector with another.
     *
     * \param in_value The other vector to subtract with.
     *
     * \return The result of the subtraction.
     */
    [[nodiscard]]
    constexpr BaseVector& Subtract(TType in_value) noexcept;

    /**
     * \brief Multiplies the current vector with another.
     *
     * \param in_value The other vector to multiply with.
     *
     * \return The result of the multiplication.
     */
    [[nodiscard]]
    constexpr BaseVector& Multiply(TType in_value) noexcept;

    /**
     * \brief Divides the current vector with another.
     * If a component of the given vector is equal to 0, the current vector equivalent component is set to 0.
     *
     * \param in_value The other vector to divide with.
     *
     * \return The result of the division.
     */
    [[nodiscard]]
    constexpr BaseVector& Divide(TType in_value) noexcept;

    /**
     * \brief Checks if the vector is equals to the given one.
     *
     * \param in_other The other vector given to perform the check.
     *
     * \return True if they are equal, false otherwise.
     */
    [[nodiscard]]
    constexpr bool IsEqual(BaseVector const& in_other) const noexcept;

    /**
     * \brief Checks if the vector is not equals to the given one.
     *
     * \param in_other The other vector given to perform the check.
     *
     * \return True if they are not equal, false otherwise.
     */
    [[nodiscard]]
    constexpr bool IsNotEqual(BaseVector const& in_other) const noexcept;

    /**
     * \brief Return the dot product value performed with another vector
     *
     * \param in_other The other vector
     *
     * \return The dot product 
     */
    [[nodiscard]]
    constexpr TType Dot(BaseVector const& in_other) const noexcept;

    /**
     * \brief Return the cross product value performed with another vector
     *
     * \param in_other The other vector
     *
     * \return The cross product
     */
    [[nodiscard]]
    constexpr BaseVector Cross(BaseVector const& in_other) const noexcept;

    #pragma endregion

    #pragma region Operators

    constexpr BaseVector& operator=(BaseVector const& in_copy) noexcept = default;
    constexpr BaseVector& operator=(BaseVector&&      in_move) noexcept = default;

    /**
     * \brief Equality operator
     * \param in_rhs Other vector
     * \return True if the 2 vectors are equals, false otherwise
     */
    [[nodiscard]]
    constexpr bool operator==(BaseVector const& in_rhs) const noexcept;

    /**
     * \brief Inequality operator
     * \param in_rhs Other vector
     * \return False if the 2 vectors are equals, true otherwise
     */
    [[nodiscard]]
    constexpr bool operator!=(BaseVector const& in_rhs) const noexcept;

    /**
     * \brief Addition operator
     * \param in_rhs Other vector
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator+(BaseVector const& in_rhs) const noexcept;

    /**
     * \brief Subtraction operator
     * \param in_rhs Other vector
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator-(BaseVector const& in_rhs) const noexcept;

    /**
     * \brief Multiplication operator
     * \param in_rhs Other vector
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator*(BaseVector const& in_rhs) const noexcept;

    /**
     * \brief Division operator
     * \param in_rhs Other vector
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator/(BaseVector const& in_rhs) const noexcept;

    /**
     * \brief Addition assignment operator
     * \param in_rhs Other vector
     * \return Instance
     */
    constexpr BaseVector& operator+=(BaseVector const& in_rhs) noexcept;

    /**
     * \brief Subtraction assignment operator
     * \param in_rhs Other vector
     * \return Instance
     */
    constexpr BaseVector& operator-=(BaseVector const& in_rhs) noexcept;

    /**
     * \brief Multiplication assignment operator
     * \param in_rhs Other vector
     * \return Instance
     */
    constexpr BaseVector& operator*=(BaseVector const& in_rhs) noexcept;

    /**
     * \brief Division assignment operator
     * \param in_rhs Other vector
     * \return Instance
     */
    constexpr BaseVector& operator/=(BaseVector const& in_rhs) noexcept;

    /**
     * \brief Scalar addition operator
     * \param in_factor Scalar operand
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator+(TType in_factor) const noexcept;

    /**
     * \brief Scalar subtraction operator
     * \param in_factor Scalar operand
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator-(TType in_factor) const noexcept;

    /**
     * \brief Scalar multiplication operator
     * \param in_factor Scalar operand
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator*(TType in_factor) const noexcept;

    /**
     * \brief Scalar division operator
     * \param in_factor Scalar operand
     * \return Resulting vector
     */
    [[nodiscard]]
    constexpr BaseVector operator/(TType in_factor) const noexcept;

    /**
     * \brief Scalar addition assignment operator
     * \param in_factor Scalar operand
     * \return Instance
     */
    constexpr BaseVector& operator+=(TType in_factor) noexcept;

    /**
     * \brief Scalar subtraction assignment operator
     * \param in_factor Scalar operand
     * \return Instance
     */
    constexpr BaseVector& operator-=(TType in_factor) noexcept;

    /**
     * \brief Scalar multiplication assignment operator
     * \param in_factor Scalar operand
     * \return Instance
     */
    constexpr BaseVector& operator*=(TType in_factor) noexcept;

    /**
     * \brief Scalar division assignment operator
     * \param in_factor Scalar operand
     * \return Instance
     */
    constexpr BaseVector& operator/=(TType in_factor) noexcept;

    /**
     * \brief Constant access operator
     * \param in_index Data index
     * \return Requested operand
     */
    [[nodiscard]]
    constexpr TType const& operator[](size_t in_index) const noexcept;

    /**
     * \brief Access operator
     * \param in_index Data index
     * \return Requested operand
     */
    [[nodiscard]]
    constexpr TType& operator[](size_t in_index) noexcept;

    /**
     * \brief Unary minus operator
     * \return Negated vector
     */
    [[nodiscard]]
    constexpr BaseVector operator-() const noexcept;

    #pragma endregion
};

#include "Maths/Vector/BaseVector.inl"

/**
 * \brief ostream <<operator overload for the BaseVector class
 *
 * \tparam TDerived Derived vector class
 * \tparam TLength Size of the vector
 * \tparam TType Underlying type of the vector
 *
 * \param inout_stream Output stream 
 * \param in_vector Vector to display
 * 
 * \return Output stream instance
 */
template <typename TDerived, size_t TLength, typename TType>
std::ostream & operator<<(std::ostream & inout_stream, BaseVector<TDerived, TLength, TType> const& in_vector)
{
    inout_stream << std::setprecision(3) << std::fixed;
    for (size_t index = 0ull; index < TLength; ++index)
        inout_stream << in_vector[index] << ' ';

    return inout_stream;
}

END_RUKEN_NAMESPACE