
#pragma once

#include "Build/Attributes.hpp"

#include "Types/StrongType/Operators/Stream/OutputStream.hpp"
#include "Types/StrongType/Operators/Stream/InputStream.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common stream operators
 * \see StrongType
 */
struct RUKEN_EMPTY_BASES Stream:
    OutputStream,
    InputStream
{};

END_RUKEN_NAMESPACE