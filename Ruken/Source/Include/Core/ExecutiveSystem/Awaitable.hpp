#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A thing that can be waited for. ex: generic events, synchronization primitives or an IO operation.
 * Awaitables cannot be waited on directly by other processing units, and must use a bridge when available.
 * \tparam TProcessingUnit Queue type the awaitable lives in.
 * \tparam TResult The result type of the awaitable.
 */
template <ProcessingUnitType TProcessingUnit, typename TResult>
struct Awaitable
{
    using ProcessingUnit = TProcessingUnit;
    using Result         = TResult;
};

END_RUKEN_NAMESPACE