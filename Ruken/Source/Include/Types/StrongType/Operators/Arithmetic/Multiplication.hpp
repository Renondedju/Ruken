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

#include "Build/Namespace.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Multiplication operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 * \tparam TAllowUnderlyingCooperation If set to true, the operators will also work directly with the underlying type. Defaults to false
 *
 * \see StrongType
 */
template <typename TStrongType, RkBool TAllowUnderlyingCooperation = true>
class Multiplication
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief Multiplication assignment operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Reference to the instance
         */
        friend constexpr TStrongType& operator*=(TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
        {
            static_cast<Type&>(in_lhs) *= static_cast<Type const&>(in_rhs);

            return in_lhs;
        }

        /**
         * \brief Multiplication assignment operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Reference to the instance
         */
        friend constexpr TStrongType& operator*=(TStrongType& in_lhs, Type const& in_rhs) noexcept
        requires TAllowUnderlyingCooperation
        {
            static_cast<Type&>(in_lhs) *= in_rhs;

            return in_lhs;
        }

        /**
         * \brief Multiplication operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator*(TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
        {
            return TStrongType(static_cast<Type const&>(in_lhs) * static_cast<Type const&>(in_rhs));
        }

        /**
         * \brief Multiplication operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator*(TStrongType const& in_lhs, Type const& in_rhs) noexcept
        requires TAllowUnderlyingCooperation
        {
            return in_lhs * TStrongType(in_rhs);
        }

        /**
         * \brief Multiplication operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator*(Type const& in_lhs, TStrongType const& in_rhs) noexcept
        requires TAllowUnderlyingCooperation
        {
            return TStrongType(in_lhs) * in_rhs;
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE
