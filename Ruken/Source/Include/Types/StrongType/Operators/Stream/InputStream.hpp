
#pragma once

#include <istream>

#include "Build/Namespace.hpp"
#include "Types/StrongType/StrongTypeHelper.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Bitwise right shift operator stream class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 * 
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
class InputStream
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
        friend constexpr std::istream& operator>>(std::istream& in_stream, TStrongType& in_strong_type) noexcept
        {
            in_stream >> static_cast<Type&>(in_strong_type);

            return in_stream;
        }

        #pragma endregion
};

END_RUKEN_NAMESPACE