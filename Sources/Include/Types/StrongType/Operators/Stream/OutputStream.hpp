#pragma once

#include <ostream>

#include "Types/StrongType/StrongTypeSuffix.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Bitwise left shift operator stream class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
struct OutputStream
{
    /**
     * \brief Bitwise left shift stream operator
     *
     * \param in_stream Stream instance
     * \param in_strong_type Value
     *
     * \return Stream instance
     */
    template <typename TStrongType>
    constexpr std::ostream& operator<<(this std::ostream& in_stream, TStrongType const& in_strong_type) noexcept
    {
        in_stream << in_strong_type.m_value << StrongTypeSuffix<TStrongType>::suffix;

        return in_stream;
    }
};

END_RUKEN_NAMESPACE