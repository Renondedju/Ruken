#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/ManualResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Asynchronous countdown latch
 */
class CountDownLatch: public ManualResetEvent
{
	#pragma region Members

	std::atomic<RkSize> m_count;

	#pragma endregion

	public:

		#pragma region Constructors

		using ManualResetEvent::ManualResetEvent;

		/**
		 * \brief Default constructor
		 * \param in_initial_count Initial latch count
		 */
		explicit CountDownLatch(RkSize in_initial_count) noexcept;

		CountDownLatch(CountDownLatch const&) = delete;
		CountDownLatch(CountDownLatch&&)      = delete;
		~CountDownLatch()                     = default;

		#pragma endregion

		#pragma region Methods

	    /**
		 * \brief Decrements the counter in a non-blocking manner
		 * Atomically decrements the internal counter by n without blocking the caller.
		 * If in_amount is greater than the value of the internal counter or is negative, the behavior is undefined.
		 * Completion is signaled when the counter reaches 0.
		 * 
		 * \param in_amount The value by which the internal counter is decreased
		 */
		RkVoid CountDown(RkSize in_amount = 1ULL) noexcept;

		#pragma endregion

		#pragma region Operators

		CountDownLatch& operator=(CountDownLatch const&) = delete;
		CountDownLatch& operator=(CountDownLatch&&)      = delete;

		#pragma endregion
};

END_RUKEN_NAMESPACE