
#pragma once

#include "Build/Namespace.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Modulo operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 *
 * \see StrongType
 */
template <typename TStrongType>
class Modulo
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief Modulo assignment operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Reference to the instance
         */
        friend constexpr TStrongType& operator%=(TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
        {
            static_cast<Type&>(in_lhs) %= static_cast<Type const&>(in_rhs);

            return in_lhs;
        }

        /**
         * \brief Modulo operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator%(TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
        {
            return TStrongType(static_cast<Type const&>(in_lhs) % static_cast<Type const&>(in_rhs));
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE
