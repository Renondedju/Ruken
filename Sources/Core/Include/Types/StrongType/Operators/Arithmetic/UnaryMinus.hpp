#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief UnaryMinus operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
struct UnaryMinus
{
    /**
     * \brief UnaryMinus operator
     * \param in_instance Operand instance
     * \return Value of the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType operator-(this TStrongType const& in_instance) noexcept
    {
        return TStrongType(-in_instance.m_value);
    }
};

END_RUKEN_NAMESPACE
