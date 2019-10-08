/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Config.hpp"

#include "Types/NamedType.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Subtraction operator class
 * 
 * This class is meant to be used in conjunction with the NamedType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongTypedef Base NamedType
 *
 * \see NamedType
 */
template <typename TStrongTypedef>
struct Subtraction
{
    /**
     * \brief Subtraction assignment operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Reference to the instance
     */
    friend constexpr TStrongTypedef& operator-=(TStrongTypedef&       in_lhs,
                                                TStrongTypedef const& in_rhs) noexcept
    {
        using Type = internal::UnderlyingType<TStrongTypedef>;
        
        static_cast<Type&>(in_lhs) -= static_cast<Type const&>(in_rhs);

        return in_lhs;
    }

    /**
     * \brief Subtraction operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Value of the new instance
     */
    friend constexpr TStrongTypedef operator-(TStrongTypedef const& in_lhs,
                                              TStrongTypedef const& in_rhs) noexcept
    {
        using Type = internal::UnderlyingType<TStrongTypedef>;
        
        return TStrongTypedef(static_cast<Type const&>(in_lhs) -
                              static_cast<Type const&>(in_rhs));
    }
};

END_DAEMON_NAMESPACE