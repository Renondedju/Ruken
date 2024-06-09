#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Increment operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
struct Increment
{
    /**
     * \brief Pre-Increment operator
     * \param in_instance Operand instance
     * \return Reference to the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType& operator++(this TStrongType& in_instance) noexcept
    {
        ++in_instance.m_value;
        return in_instance;
    }

    /**
     * \brief Post-Increment operator
     * \param in_instance Operand instance
     * \return Value of the new instance
     */
    template <typename TStrongType>
    constexpr TStrongType operator++(this TStrongType& in_instance, int) noexcept
    {
        return TStrongType(in_instance.m_value++);
    }
};

END_RUKEN_NAMESPACE
