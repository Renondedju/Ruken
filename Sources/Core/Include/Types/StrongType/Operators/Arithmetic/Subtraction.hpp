#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Subtraction operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
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
    template <typename TStrongType>
    constexpr TStrongType& operator-=(this TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
    {
        in_lhs.m_value -= in_rhs.m_value;

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
    template <typename TStrongType>
    constexpr TStrongType operator-(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
    {
        return TStrongType(in_lhs.m_value - in_rhs.m_value);
    }
};

END_RUKEN_NAMESPACE
