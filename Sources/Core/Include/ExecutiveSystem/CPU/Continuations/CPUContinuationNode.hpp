#pragma once

#include "Types/FundamentalTypes.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

struct CPUContinuationNode;
using  CPUContinuationNodePtr = std::atomic<CPUContinuationNode*>;

struct CPUContinuationNode
{
	static inline auto locked    {reinterpret_cast<CPUContinuationNode* const>(0x1)};
	static inline auto consumed  {reinterpret_cast<CPUContinuationNode* const>(0x2)};

	CPUContinuationNodePtr* head {nullptr};  ///< Reference to the head of the list
	CPUContinuationNodePtr  next {nullptr}; ///< Next subscription in the list

	/**
	 * \brief Attempts a suspension by attaching the awaiter to the awaited event
	 * \return True if the suspension succeeded, false otherwise
	 */
	[[nodiscard]]
	RkBool TryStartAwait() noexcept;

	/**
	 * \brief Detaches the continuation
	 */
	RkBool TryStopAwait() noexcept;

	/**
	 * \brief Checks if the awaitable we want to wait for has been completed already
	 * \note This function can be called even if the awaited event has been deleted already
	 * \return True if the awaiter has been completed, false otherwise
	 */
	[[nodiscard]]
	RkBool Consumed() const noexcept;
};

END_RUKEN_NAMESPACE