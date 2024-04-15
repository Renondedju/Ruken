
#pragma once

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Division operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 * \tparam TAllowUnderlyingCooperation If set to true, the operators will also work directly with the underlying type. Defaults to false
 *
 * \see StrongType
 */
template <typename TStrongType, RkBool TAllowUnderlyingCooperation = true>
class Division
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief Division assignment operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Reference to the instance
         */
        friend constexpr TStrongType& operator/=(TStrongType& in_lhs, TStrongType const& in_rhs) noexcept
        {
            static_cast<Type&>(in_lhs) /= static_cast<Type const&>(in_rhs);

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
        friend constexpr TStrongType& operator/=(TStrongType& in_lhs, Type const& in_rhs) noexcept
        requires TAllowUnderlyingCooperation
        {
            static_cast<Type&>(in_lhs) /= in_rhs;

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
        friend constexpr TStrongType operator/(TStrongType const& in_lhs, TStrongType const& in_rhs) noexcept
        {
            return TStrongType(static_cast<Type const&>(in_lhs) / static_cast<Type const&>(in_rhs));
        }

        /**
         * \brief Division operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator/(TStrongType const& in_lhs, Type const& in_rhs) noexcept
        requires TAllowUnderlyingCooperation
        {
            return in_lhs / TStrongType(in_rhs);
        }

        /**
         * \brief Division operator
         *
         * \param in_lhs Left-hand side operand
         * \param in_rhs Right-hand side operand
         *
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator/(Type const& in_lhs, TStrongType const& in_rhs) noexcept
        requires TAllowUnderlyingCooperation
        {
            return TStrongType(in_lhs) / in_rhs;
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE
