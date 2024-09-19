#pragma once

#include "ExecutiveSystem/CPU/Queues/CPUQueue.hpp"
#include "Meta/TupleHasType.hpp"

#include <type_traits>
#include <coroutine>
#include <variant>

BEGIN_RUKEN_NAMESPACE

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUPromise;

template <typename TType > struct VariantHelper										      { using ValueType = TType;  using HasExceptions = std::false_type;};
template <typename TValue> struct VariantHelper<std::variant<std::exception_ptr, TValue>> { using ValueType = TValue; using HasExceptions = std::true_type; };
template <typename TValue> struct VariantHelper<std::variant<TValue, std::exception_ptr>> { using ValueType = TValue; using HasExceptions = std::true_type; };

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskContinuationBase: CPUContinuation<TResult>
{
	static constexpr RkBool has_exceptions = VariantHelper<TResult>::HasExceptions::value || std::is_same_v<TResult, std::exception_ptr>;

	#pragma region Lifetime

	/**
	 * @brief Default constructor
	 * @param in_promise Owning coroutine instance
	 */
	template <typename TCoroutineResult>
	explicit CPUTaskContinuationBase(CPUTaskPromise<TQueueHandle, TCoroutineResult>& in_promise) noexcept:
		CPUContinuation<TResult> {},
		m_coroutine				 {std::coroutine_handle<CPUTaskPromise<TQueueHandle, TCoroutineResult>>::from_promise(in_promise)}
	{ }

	CPUTaskContinuationBase (CPUTaskContinuationBase const&) = default;
	CPUTaskContinuationBase (CPUTaskContinuationBase&&     ) = default;
	~CPUTaskContinuationBase()								 = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * \brief Checks if the wait has been completed already
	 * \note This function can be called even if the awaited event has been deleted already
	 * \return True if the awaiter has been completed, false otherwise
	 */
	[[nodiscard]]
	RkBool await_ready() const noexcept
	{ return this->Consumed(); }

	/**
	 * \brief Attempts a suspension by attaching the awaiter to the awaited event
	 * \return True if the suspension succeeded, false otherwise
	 */
	[[nodiscard]]
	RkBool await_suspend(std::coroutine_handle<>) noexcept
	{ return this->TryStartAwait(); }

	#pragma endregion

	protected:

		std::coroutine_handle<> m_coroutine;
};

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskContinuation: CPUTaskContinuationBase<TQueueHandle, TResult>
{
    /**
     * @brief Default constructor
     * @param in_awaiter Owning coroutine instance
     * @param in_awaited Reference to the awaited event
     */
	template <typename TCoroutineResult>
    CPUTaskContinuation(CPUAwaitable<TResult> const& in_awaited, CPUTaskPromise<TQueueHandle, TCoroutineResult>& in_awaiter) noexcept:
		CPUTaskContinuationBase<TQueueHandle, TResult> {in_awaiter}
    { this->Setup(in_awaited, *this); }

	/// @brief Returns the result of the wait
    auto await_resume() const noexcept(!CPUTaskContinuationBase<TQueueHandle, TResult>::has_exceptions)
    {
        if constexpr (VariantHelper<TResult>::HasExceptions::value)
	        if (std::get<std::exception_ptr>(return_value))
	        	std::rethrow_exception(std::get<std::exception_ptr>(return_value));

    	if constexpr (std::is_same_v<TResult, std::exception_ptr>)
    		if (return_value)
    			std::rethrow_exception(return_value);

        if constexpr (VariantHelper<TResult>::HasExceptions::value)
			return std::move(std::get<VariantHelper<TResult>::ValueType>(return_value));

		if constexpr (!std::is_same_v<TResult, std::exception_ptr>)
    		return return_value;
    }

	/// @brief Called when the wait is over
	RkVoid Signal(TResult const& in_value) noexcept
	{
		return_value = in_value;
		TQueueHandle::GetInstance().Push(this->m_coroutine);
	}

	protected:

		TResult	return_value {};
};

template <CQueueHandle TQueueHandle>
struct CPUTaskContinuation<TQueueHandle, RkVoid>: CPUTaskContinuationBase<TQueueHandle, RkVoid>
{
	/**
	 * @brief Default constructor
	 * @param in_awaiter Owning coroutine instance
	 * @param in_awaited Reference to the awaited event
	 */
	template <typename TCoroutineResult>
	CPUTaskContinuation(CPUAwaitable<RkVoid> const& in_awaited, CPUTaskPromise<TQueueHandle, TCoroutineResult>& in_awaiter) noexcept:
		CPUTaskContinuationBase<TQueueHandle, RkVoid> {in_awaiter}
	{ this->Setup(in_awaited, *this); }

	/// @brief Returns the result of the wait
	static constexpr void await_resume() noexcept
	{ }

	/// @brief Called when the wait is over
	RkVoid Signal() const noexcept
	{ TQueueHandle::GetInstance().Push(this->m_coroutine); }
};

END_RUKEN_NAMESPACE
