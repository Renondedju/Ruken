#pragma once

#include "ExecutiveSystem/CPU/Queues/CPUQueue.hpp"
#include "Meta/TupleHasType.hpp"

#include <type_traits>
#include <coroutine>
#include <variant>

BEGIN_RUKEN_NAMESPACE

template <typename TResult>
struct CPUPromise;

template <typename TType > struct VariantHelper										      { using ValueType = TType;  using HasExceptions = std::false_type;};
template <typename TValue> struct VariantHelper<std::variant<std::exception_ptr, TValue>> { using ValueType = TValue; using HasExceptions = std::true_type; };
template <typename TValue> struct VariantHelper<std::variant<TValue, std::exception_ptr>> { using ValueType = TValue; using HasExceptions = std::true_type; };

template <typename TResult>
struct CPUCoroutineContinuationBase: CPUContinuation<TResult>
{
	static constexpr RkBool has_exceptions = VariantHelper<TResult>::HasExceptions::value || std::is_same_v<TResult, std::exception_ptr>;

	#pragma region Lifetime

	/**
	 * @brief Default constructor
	 * @param in_promise Owning coroutine instance
	 */
	template <typename TCoroutineResult>
	explicit CPUCoroutineContinuationBase(CPUPromise<TCoroutineResult>& in_promise) noexcept:
		CPUContinuation<TResult> {},
		m_queue					 {*in_promise.CurrentQueue()},
		m_coroutine				 {std::coroutine_handle<CPUPromise<TCoroutineResult>>::from_promise(in_promise)}
	{ }

	CPUCoroutineContinuationBase (CPUCoroutineContinuationBase const&) = default;
	CPUCoroutineContinuationBase (CPUCoroutineContinuationBase&&     ) = default;
	~CPUCoroutineContinuationBase()									   = default;

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

		CPUQueue&				m_queue;
		std::coroutine_handle<> m_coroutine;
};

template <typename TResult>
struct CPUCoroutineContinuation: CPUCoroutineContinuationBase<TResult>
{
    /**
     * @brief Default constructor
     * @param in_awaiter Owning coroutine instance
     * @param in_awaited Reference to the awaited event
     */
	template <typename TCoroutineResult>
    CPUCoroutineContinuation(CPUAwaitable<TResult> const& in_awaited, CPUPromise<TCoroutineResult>& in_awaiter) noexcept:
		CPUCoroutineContinuationBase<TResult> {in_awaiter}
    { this->Setup(in_awaited, *this); }

	/// @brief Returns the result of the wait
    auto await_resume() const noexcept(!CPUCoroutineContinuationBase<TResult>::has_exceptions)
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
		this->m_queue.Push(this->m_coroutine);
	}

	protected:

		TResult	return_value {};
};

template <>
struct CPUCoroutineContinuation<RkVoid>: CPUCoroutineContinuationBase<RkVoid>
{
	/**
	 * @brief Default constructor
	 * @param in_awaiter Owning coroutine instance
	 * @param in_awaited Reference to the awaited event
	 */
	template <typename TCoroutineResult>
	CPUCoroutineContinuation(CPUAwaitable<RkVoid> const& in_awaited, CPUPromise<TCoroutineResult>& in_awaiter) noexcept:
		CPUCoroutineContinuationBase {in_awaiter}
	{ this->Setup(in_awaited, *this); }

	/// @brief Returns the result of the wait
	static constexpr void await_resume() noexcept
	{ }

	/// @brief Called when the wait is over
	RkVoid Signal() const noexcept
	{ this->m_queue.Push(this->m_coroutine); }
};

END_RUKEN_NAMESPACE
