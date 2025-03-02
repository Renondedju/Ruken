#pragma once

#include "ExecutiveSystem/Concepts/CQueueHandle.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Tasks/CTaskResult.hpp"
#include "ExecutiveSystem/CPU/Awaitables/Primitives/ManualResetEvent.hpp"

#include "Debug/Trace.hpp"

#include <source_location>
#include <variant>

BEGIN_RUKEN_NAMESPACE
#define RUKEN_INTERNAL_SOURCE_LOCATION [[maybe_unused]] std::source_location in_source_location = std::source_location::current()

template <CQueueHandle TQueueHandle, CTaskResult TResult>
struct CPUTask;

template <typename TResult>
using TPromiseAwaitableValue = std::conditional_t<
		!std::is_same_v<TResult, RkVoid>,
			std::variant<TResult, std::exception_ptr>,
			std::exception_ptr
	>;

template <typename TResult> struct CPUAwaitableStorage		   { TResult result; };
template <>					struct CPUAwaitableStorage<RkVoid> {};

/**
 * \brief Implements the base common behavior for all CPU tasks
 * \tparam TResult Return type of the associated coroutine
 */
template <CQueueHandle TQueueHandle, CTaskResult TResult>
struct CPUTaskPromiseBase: CPUAwaitableStorage<TPromiseAwaitableValue<TResult>>, ManualResetEvent
{
	using ReturnType     = TPromiseAwaitableValue<TResult>;
	using ProcessingUnit = CentralProcessingUnit;
	using Storage        = CPUAwaitableStorage<TPromiseAwaitableValue<TResult>>;

    #pragma region Coroutine Methods

	/**
     * \brief Converts awaited types to asynchronous events if possible
     * \param in_self Task instance
     * \param in_awaitable Asynchronous event instance
     * \return Awaiter instance
     */
	template<typename TThis, typename TAwaitable>
	auto await_transform(this TThis& in_self, TAwaitable const& in_awaitable, RUKEN_INTERNAL_SOURCE_LOCATION) noexcept;

	// Coroutine lifetime
	template <typename TThis>
	auto initial_suspend    (this TThis&, RUKEN_INTERNAL_SOURCE_LOCATION) noexcept;
	auto final_suspend		() noexcept;
	void unhandled_exception() noexcept;

	#pragma endregion

	#pragma region Members

	protected:

		template <CQueueHandle TOtherQueueHandle, CTaskResult TOtherResult>
		friend struct CPUTask;

		std::atomic<RkSize> m_references {1ULL};

		#ifdef RUKEN_TRACE_BUILD
			TracyCZoneCtx m_zone {};
		#endif

	#pragma endregion
};

template<CQueueHandle TQueueHandle, CTaskResult TResult>
struct CPUTaskPromise: CPUTaskPromiseBase<TQueueHandle, TResult>
{
	auto operator co_await() noexcept;
	auto get_return_object() noexcept;
	void return_value     (TResult const& in_result) noexcept;
};

template <CQueueHandle TQueueHandle>
struct CPUTaskPromise<TQueueHandle, RkVoid>: CPUTaskPromiseBase<TQueueHandle, RkVoid>
{
	auto operator co_await() noexcept;
	auto get_return_object() noexcept;
	void return_void	  () noexcept;
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUTaskPromise.inl"
