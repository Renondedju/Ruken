#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingUnitType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A thing that can be waited for. ex: generic events, synchronization primitives or an IO operation.
 * Awaitables cannot be waited on directly by other processing units, and must use a bridge when available.
 *
 * \tparam TProcessingUnit Queue type the awaitable lives in.
 * \tparam TResult The result type of the awaitable.
 * \tparam TReliable When false, waiting for this object can result in a failure fail.
 *         The result of the awaitable might not be valid after a failure and reading from it should be avoided.
 */
template <ProcessingUnitType TProcessingUnit, typename TResult, RkBool TReliable>
struct Awaitable
{
    using ProcessingUnit = TProcessingUnit;
    using Result         = TResult;

    /// When false, waiting for this object can result in a failure fail.
    static constexpr RkBool reliable {TReliable};
};

END_RUKEN_NAMESPACE