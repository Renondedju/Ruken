#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Primitives/ManualResetEvent.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Asynchronous countdown latch
 */
class CountDownLatch: public CPUAwaiter,
                      public CPUAwaitable<RkVoid, true>
{
	#pragma region Members

	std::atomic<RkSize> m_count;

	#pragma endregion

	public:

		using ProcessingUnit = CentralProcessingUnit;

		#pragma region Lifetime

		/**
		 * \brief Default constructor
		 * \param in_initial_count Initial latch count
		 */
		explicit CountDownLatch(RkSize in_initial_count) noexcept;

		CountDownLatch(CountDownLatch const&) = delete;
		CountDownLatch(CountDownLatch&&)      = delete;
		~CountDownLatch() override            = default;
	
		CountDownLatch& operator=(CountDownLatch const&) = delete;
		CountDownLatch& operator=(CountDownLatch&&)      = delete;

		#pragma endregion

		#pragma region Methods

		RkVoid Deallocate() override
	    {}

        /**
         * \brief Decrements the internal counter by one.
         *		  When the counter reaches 0, the completion of the latch is signaled.
         *		  Calling this function when the internal counter is already 0 will cause an underflow.
         */
        RkVoid CountDown() noexcept;

        /**
         * \brief Returns the current value of the internal counter.
         * \warning Synchronization cannot be achieved with this function.
         */
        RkSize Value() const noexcept;

	    /**
         * \brief Called when an awaited event has been completed and counts down.
         */
		RkVoid OnAwaitedContinuation() noexcept override;

		#pragma endregion
};

END_RUKEN_NAMESPACE