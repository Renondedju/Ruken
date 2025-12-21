#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Equal operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
struct Equal
{
    /**
     * \brief Equal operator
     *
     * \param in_lhs Left-hand side operand
     * \param in_rhs Right-hand side operand
     *
     * \return True if the left operand is equal to the right operand, false otherwise.
     */
    template <typename TStrongType>
    constexpr bool operator==(this TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
    {
    	using TBase = TStrongType::TUnderlying;

        return static_cast<TBase>(in_lhs) == static_cast<TBase>(in_rhs);
    }
};

END_RUKEN_NAMESPACE