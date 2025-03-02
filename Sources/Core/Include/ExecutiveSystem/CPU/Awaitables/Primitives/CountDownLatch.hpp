#pragma once

#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Primitives/ManualResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Asynchronous countdown latch
 */
struct CountDownLatch: ManualResetEvent
{
	/**
	 * \brief Default constructor
	 * \param in_initial_count Initial latch count
	 */
	explicit CountDownLatch(RkSize in_initial_count) noexcept;

	/**
	 * @brief Decrements the internal counter by one.
	 *		  When the counter reaches 0, the completion of the latch is signaled.
	 *		  Calling this function when the internal counter is already 0 will cause an underflow.
	 */
	RkVoid Signal() noexcept;

    /**
     * \brief Returns the current value of the internal counter.
     * \warning Synchronization cannot be achieved with this function.
     */
    RkSize Value() const noexcept;

	private:

		std::atomic<RkSize>	m_counter;
};

END_RUKEN_NAMESPACE