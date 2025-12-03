#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Division operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 *
 * \tparam TAllowUnderlyingCooperation If set to true, the operators will also work directly with the underlying type. Defaults to false
 *
 * \see StrongType
 */
template <RkBool TAllowUnderlyingCooperation = true>
struct Division
{
    /**
     * \brief Division assignment operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Reference to the instance
     */
    template <typename TStrongType>
    constexpr TStrongType& operator/=(this TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
    {
        in_lhs = in_lhs / in_rhs;
        return in_lhs;
    }

    /**
     * \brief Division assignment operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Reference to the instance
     */
    template <typename TStrongType>
    constexpr TStrongType& operator/=(this TStrongType& in_lhs, TStrongType::TUnderlying const& in_rhs) noexcept
    {
        in_lhs = in_lhs / in_rhs;
        return in_lhs;
    }

    /**
     * \brief Division operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Value of the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType operator/(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
    {
        using TBase = TStrongType::TUnderlying;

        return TStrongType(static_cast<TBase>(in_lhs) / static_cast<TBase>(in_rhs));
    }

    // --- Underlying Cooperation

    /**
     * \brief Division operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return Value of the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType operator/(this TStrongType const& in_lhs, TStrongType::TUnderlying const& in_rhs) noexcept
    requires TAllowUnderlyingCooperation
    {
        return in_lhs / TStrongType(in_rhs);
    }
};

END_RUKEN_NAMESPACE
