
#pragma once

#include <ostream>

#include "Types/StrongType/StrongTypeSuffix.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Bitwise left shift operator stream class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
class OutputStream
{
    using Type = StrongTypeHelper::UnderlyingType<TStrongType>;

    public:

        #pragma region Operators

        /**
         * \brief Bitwise left shift stream operator
         *
         * \param in_stream Stream instance
         * \param in_strong_type Value
         *
         * \return Stream instance
         */
        friend constexpr std::ostream& operator<<(std::ostream& in_stream, TStrongType const& in_strong_type) noexcept
        {
            in_stream << static_cast<Type const&>(in_strong_type) << StrongTypeSuffix<TStrongType>::suffix;

            return in_stream;
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE