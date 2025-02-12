#pragma once

#include "Build/BuildInfo.hpp"
#include "ExecutiveSystem/Concepts/CQueueHandle.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

#include "Debug/Trace.hpp"

#include <source_location>
#include <variant>

BEGIN_RUKEN_NAMESPACE

#define RUKEN_INTERNAL_SOURCE_LOCATION [[maybe_unused]] std::source_location in_source_location = std::source_location::current()

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTask;

template <typename TResult>
using TPromiseAwaitableValue = std::conditional_t<
		!std::is_same_v<TResult, RkVoid>,
			std::variant<TResult, std::exception_ptr>,
			std::exception_ptr
	>;


/**
 * \brief Implements the base common behavior for all CPU tasks
 * \tparam TResult Return type of the associated coroutine
 */
template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskPromiseBase:
	protected CPUAwaitableStorage<TPromiseAwaitableValue<TResult>>,
			  CPUAwaitable       <TPromiseAwaitableValue<TResult>>
{
	using ReturnType     = TPromiseAwaitableValue<TResult>;
	using ProcessingUnit = CentralProcessingUnit;

	CPUTaskPromiseBase() noexcept:
		CPUAwaitableStorage<TPromiseAwaitableValue<TResult>> {},
		CPUAwaitable	   <TPromiseAwaitableValue<TResult>> {m_awaiter_list, std::addressof(value)}
	{}

    #pragma region Coroutine Methods

	/**
     * \brief Converts awaited types to asynchronous events if possible
     * \param in_self Task instance
     * \param in_awaitable Asynchronous event instance
     * \return Awaiter instance
     */
	template<typename TThis, typename TAwaitable>
	auto await_transform    (this TThis& in_self, TAwaitable const& in_awaitable, RUKEN_INTERNAL_SOURCE_LOCATION) noexcept;

	// Coroutine lifetime
	template <typename TThis>
	auto initial_suspend    (this TThis&, RUKEN_INTERNAL_SOURCE_LOCATION) noexcept;
	auto final_suspend		() noexcept;
	void unhandled_exception() noexcept;

	#pragma endregion

	#pragma region Members

	protected:

		template <CQueueHandle TOtherQueueHandle, typename TOtherResult>
		friend struct CPUTask;

		using CPUAwaitableStorage<TPromiseAwaitableValue<TResult>>::value;

		std::atomic<RkSize>								m_references   {1ULL};
		CPUAwaiterList<TPromiseAwaitableValue<TResult>> m_awaiter_list {nullptr};

		#ifdef RUKEN_TRACE_BUILD
			TracyCZoneCtx m_zone {};
		#endif

	#pragma endregion
};

template<CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskPromise: CPUTaskPromiseBase<TQueueHandle, TResult>
{
	auto get_return_object() noexcept;
	void return_value     (TResult const& in_result) noexcept;
};

template <CQueueHandle TQueueHandle>
struct CPUTaskPromise<TQueueHandle, RkVoid>: CPUTaskPromiseBase<TQueueHandle, RkVoid>
{
	auto get_return_object() noexcept;
	void return_void	  () noexcept;
};

END_RUKEN_NAMESPACE

#include "ExecutiveSystem/CPU/Awaitables/CPUTaskPromise.inl"
