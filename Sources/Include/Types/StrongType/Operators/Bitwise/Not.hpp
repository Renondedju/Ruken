#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Bitwise NOT operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 *
 * \see StrongType
 */

class Not
{
    /**
     * \brief Bitwise NOT operator
     * \param in_instance Operand instance
     * \return Value of the new instance
     */
	template <typename TStrongType>
    constexpr TStrongType operator~(this TStrongType const& in_instance) noexcept
    {
		using TBase = typename TStrongType::TUnderlying;

        return TStrongType(~static_cast<TBase>(in_instance));
    }
};

END_RUKEN_NAMESPACE