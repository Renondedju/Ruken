
#pragma once

#include "Build/Namespace.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief UnaryMinus operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 *
 * \see StrongType
 */
template <typename TStrongType>
class UnaryMinus
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief UnaryMinus operator
         * \param in_instance Operand instance
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator-(TStrongType const& in_instance) noexcept
        {
            return TStrongType(-static_cast<Type const&>(in_instance));
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE
