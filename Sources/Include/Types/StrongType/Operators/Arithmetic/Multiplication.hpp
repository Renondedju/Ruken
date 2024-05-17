#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Multiplication operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 *
 * \tparam TAllowUnderlyingCooperation If set to true, the operators will also work directly with the underlying type.
 */
template <RkBool TAllowUnderlyingCooperation = true>
struct Multiplication
{
    /**
     * \brief Multiplication assignment operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Reference to the instance
     */
    template <typename TStrongType>
    constexpr TStrongType& operator*=(this TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
    {
        in_lhs = in_lhs * in_rhs;
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
    template <typename TStrongType>
    constexpr TStrongType& operator*=(this TStrongType& in_lhs, typename TStrongType::TUnderlying const& in_rhs) noexcept
    {
        in_lhs = in_lhs * in_rhs;
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
    template <typename TStrongType>
    constexpr TStrongType operator*(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
    {
        using TBase = typename TStrongType::TUnderlying;

        return TStrongType(static_cast<TBase>(in_lhs) * static_cast<TBase>(in_rhs));
    }

    // --- Underlying Cooperation

    /**
     * \brief Multiplication operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Value of the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType operator*(this TStrongType const& in_lhs, typename TStrongType::TUnderlying const& in_rhs) noexcept
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
    template <typename TStrongType>
    constexpr TStrongType operator*(this typename TStrongType::UnderlyingType const& in_lhs, TStrongType const& in_rhs) noexcept
    requires TAllowUnderlyingCooperation
    {
        return TStrongType(in_lhs) * in_rhs;
    }
};

END_RUKEN_NAMESPACE
