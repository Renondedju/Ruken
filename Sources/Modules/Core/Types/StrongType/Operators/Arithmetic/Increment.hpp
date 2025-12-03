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
        using TBase = TStrongType::TUnderlying;

        ++static_cast<TBase>(in_instance);
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
        using TBase = TStrongType::TUnderlying;

        return TStrongType(static_cast<TBase>(in_instance)++);
    }
};

END_RUKEN_NAMESPACE
