#pragma once

#include "Build/Namespace.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/CountDownLatch.hpp"

BEGIN_RUKEN_NAMESPACE

//struct WhenAllSuspension final: CPUSuspension
//{
//    CountDownLatch& latch;
//
//    #pragma region Initialization
//
//    /**
//     * \brief Default constructor
//     * \param in_head  Head node, used for insertion
//     * \param in_latch Latch to decrement on completion
//     * \param in_completion_callback Awaiter's suspension completion callback
//     */
//    WhenAllSuspension(Node& in_head, CountDownLatch& in_latch, CPUAwaitableCallback& in_completion_callback) noexcept:
//        CPUSuspension {.head = in_head, .completion_callback = in_completion_callback}, latch {in_latch}
//    {}
//
//    WhenAllSuspension(WhenAllSuspension const&) = delete;
//	WhenAllSuspension(WhenAllSuspension&&)      = delete;
//    ~WhenAllSuspension()                        = default;
//
//    WhenAllSuspension& operator=(WhenAllSuspension const&) = delete;
//	WhenAllSuspension& operator=(WhenAllSuspension&&)      = delete;
//
//    #pragma endregion
//
//    #pragma region Methods
//
//    /**
//     * \brief Called when the awaited event has been completed
//     * Tasks are simply pushed back to their original queue,
//     * waiting for a worker to pick it up and resume it
//     */
//    RkVoid OnCompletion() noexcept
//    { latch.CountDown(); }
//
//    /**
//     * \brief Returns the value of the suspension when resumed.
//     */
//    static constexpr RkVoid await_resume() noexcept { }
//
//    #pragma endregion
//};
//
//template <RkSize TSize>
//struct WhenAll: CountDownLatch
//{
//    #pragma region Initialization
//
//    WhenAll(CPUAwaitable in_awaitables[TSize]) noexcept:
//        CountDownLatch(TSize)
//    {
//        for (RkSize index = 0ULL; index < TSize; ++index)
//            WhenAllSuspension(in_awaitables[index].GetSuspensionNode(), *this);
//    }
//
//    WhenAll(WhenAll const&) = default;
//	WhenAll(WhenAll&&)      = default;
//    ~WhenAll()              = default;
//
//    WhenAll& operator=(WhenAll const&) = default;
//	WhenAll& operator=(WhenAll&&)      = default;
//
//    #pragma endregion
//};


END_RUKEN_NAMESPACE