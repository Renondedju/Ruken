
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Defines the type of protection of a (multi-threaded) resource
 * 
 * Read  => Read  protection
 * Write => Write protection
 */
enum class EAccessMode : RkByte
{
    Read,
    Write,
    Unsafe
};

END_RUKEN_NAMESPACE