
#pragma once

#include "Build/Namespace.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Increment operator class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 *
 * \see StrongType
 */
template <typename TStrongType>
class Increment
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief Pre-Increment operator
         * \param in_instance Operand instance
         * \return Reference to the new instance
         */
        friend constexpr TStrongType operator++(TStrongType& in_instance) noexcept
        {
            return ++static_cast<Type&>(in_instance);
        }

        /**
         * \brief Post-Increment operator
         * \param in_instance Operand instance
         * \return Value of the new instance
         */
        friend constexpr TStrongType operator++(TStrongType& in_instance, int) noexcept
        {
            return static_cast<Type&>(in_instance)++;
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE
