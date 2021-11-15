
#pragma once

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/StrongType/Operators/Stream/OutputStream.hpp"
#include "Types/StrongType/Operators/Stream/InputStream.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Module gathering all common stream operators
 * \tparam TStrongType Base StrongType
 * \see StrongType
 */
template <typename TStrongType>
struct RUKEN_EMPTY_BASES Stream:
    OutputStream<TStrongType>,
    InputStream <TStrongType>
{};

END_RUKEN_NAMESPACE