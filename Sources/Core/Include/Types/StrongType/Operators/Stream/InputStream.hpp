#pragma once

#include <istream>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Bitwise right shift operator stream class
 * 
 * This class is meant to be used in conjunction with the StrongType class.
 * This allows for better and quicker operator integrations to named types.
 */
struct InputStream
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
    constexpr std::istream& operator>>(this std::istream& in_stream, TStrongType& in_strong_type) noexcept
    {
        in_stream >> in_strong_type.m_value;

        return in_stream;
    }
};

END_RUKEN_NAMESPACE