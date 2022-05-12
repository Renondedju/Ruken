#pragma once

#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingQueueType TQueue>
struct Task;

template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue>
class Subscription;

// ---

/**
 * \brief Checks if the passed type is a valid awaiter
 * \tparam TType Type to check
 */
template <typename TType>
struct IsAwaiterType : std::false_type
{};

template <ProcessingQueueType TOwningQueue, ProcessingQueueType TAwaitedQueue>
struct IsAwaiterType<Subscription<TOwningQueue, TAwaitedQueue>>
{
    static constexpr RkBool value = std::constructible_from<
        Subscription<TOwningQueue, TAwaitedQueue>,
        Task<TOwningQueue> const&, AsynchronousEvent<TAwaitedQueue>&
    >;
};

template <typename TType>
concept AwaiterType = IsAwaiterType<TType>::value;

END_RUKEN_NAMESPACE