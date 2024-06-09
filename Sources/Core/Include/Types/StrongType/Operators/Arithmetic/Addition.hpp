#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Addition operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 *
 * \see StrongType
 */
struct Addition
{
    /**
     * \brief Addition assignment operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     * \tparam TStrongType Base StrongType
     *
     * \return Reference to the instance
     */
    template <typename TStrongType>
    constexpr TStrongType& operator+=(this TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
    {
        in_lhs = in_lhs + in_rhs;

        return in_lhs;
    }

    /**
     * \brief Addition operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     * \tparam TStrongType Base StrongType
     *
     * \return Value of the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType operator+(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
    {
		using TBase = typename TStrongType::Tunderlying;

        return TStrongType(static_cast<TBase>(in_lhs) + static_cast<TBase>(in_rhs));
    }
};

END_RUKEN_NAMESPACE