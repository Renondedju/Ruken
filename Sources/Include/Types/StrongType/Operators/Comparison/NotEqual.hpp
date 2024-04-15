
#pragma once

#include "Build/Namespace.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief NotEqual operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 *
 * \see StrongType
 */
template <typename TStrongType>
struct NotEqual
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief NotEqual operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return True if the left operand is not equal to the right operand, false otherwise.
         */
        friend constexpr bool operator!=(TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
        {
            return static_cast<Type const&>(in_lhs) != static_cast<Type const&>(in_rhs);
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE