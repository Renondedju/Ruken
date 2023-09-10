#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CountDownLatch.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Algorythms/WhenAll.hpp"

BEGIN_RUKEN_NAMESPACE
    struct ForEachSuspension final: CPUAutomaticContinuation
{
    CountDownLatch& latch;

    #pragma region Initialization

    /**
     * \brief Default constructor
     * \param in_head  Head node, used for insertion
     * \param in_latch Latch to decrement on completion
     */
    ForEachSuspension(Node& in_head, CountDownLatch& in_latch) noexcept: CPUAutomaticContinuation {in_head}, latch {in_latch} {}
    ForEachSuspension(ForEachSuspension const&) = delete;
	ForEachSuspension(ForEachSuspension&&)      = delete;
    ~ForEachSuspension() override               = default;

    ForEachSuspension& operator=(ForEachSuspension const&) = delete;
	ForEachSuspension& operator=(ForEachSuspension&&)      = delete;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Called when the awaited event has been completed
     * Tasks are simply pushed back to their original queue,
     * waiting for a worker to pick it up and resume it
     */
    RkVoid OnCompletion() noexcept override
    { latch.CountDown(); }

    /**
     * \brief Returns the value of the suspension when resumed.
     */
    static constexpr RkVoid await_resume() noexcept { }

    #pragma endregion
};

struct ForEach: CountDownLatch
{
    #pragma region Initialization

    WhenAll(RkSize const in_item_count, ) noexcept:
        CountDownLatch(in_item_count)
    {
        for (RkSize index = 0ULL; index < TSize; ++index)
            WhenAllSuspension(in_awaitables[index].GetSuspensionNode(), *this);
    }

    WhenAll(WhenAll const&) = default;
	WhenAll(WhenAll&&)      = default;
    ~WhenAll()              = default;

    WhenAll& operator=(WhenAll const&) = default;
	WhenAll& operator=(WhenAll&&)      = default;

    #pragma endregion
};


END_RUKEN_NAMESPACE