#pragma once

#include <coroutine>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief CentralProcessingQueue interface.
 * Use this class as a handle to a central processing queue
 *
 * \warning This is only a TEMPORARY solution and relies on compiler code duplication.
 * \warning Eventually it will get fixed to only store a queue reference but that requires some 
 * \warning templates-related sorting and allocation logic I don't want to tackle yet
 */
struct ICentralProcessingQueue
{
    using TryDequeueSignature = RkBool(*)(std::coroutine_handle<>&, RkSize) noexcept;
    using EnqueueSignature    = RkVoid(*)(std::coroutine_handle<>&&)        noexcept;
    using EmptySignature      = RkBool(*)()                                 noexcept;

    #pragma region Members

    const TryDequeueSignature try_dequeue;
    const EnqueueSignature    enqueue;
    const EmptySignature      empty;

    #pragma endregion
};

END_RUKEN_NAMESPACE