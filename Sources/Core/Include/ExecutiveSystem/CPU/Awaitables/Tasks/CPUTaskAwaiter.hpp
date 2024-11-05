#pragma once

#include "../../../../Meta/TupleHasType.hpp"
#include "../../Queues/CPUQueue.hpp"
#include "../CPUAwaiter.hpp"

#include <type_traits>
#include <coroutine>
#include <variant>


BEGIN_RUKEN_NAMESPACE

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskPromiseBase;

template <typename TType > struct VariantHelper										      { using ValueType = TType;  using HasExceptions = std::false_type;};
template <typename TValue> struct VariantHelper<std::variant<std::exception_ptr, TValue>> { using ValueType = TValue; using HasExceptions = std::true_type; };
template <typename TValue> struct VariantHelper<std::variant<TValue, std::exception_ptr>> { using ValueType = TValue; using HasExceptions = std::true_type; };

template <typename TResult>
struct CPUTaskAwaiterBase: CPUAwaiter<TResult>
{
	static constexpr RkBool has_exceptions = VariantHelper<TResult>::HasExceptions::value || std::is_same_v<TResult, std::exception_ptr>;

	#pragma region Lifetime

	/**
	 * @brief Default constructor
	 * @param in_awaitable Awaited event
	 */
	explicit CPUTaskAwaiterBase(CPUAwaitable<TResult> const& in_awaitable) noexcept:
		CPUAwaiter<TResult>(),
		m_awaitable {in_awaitable}
	{}

	CPUTaskAwaiterBase (CPUTaskAwaiterBase const&) = default;
	CPUTaskAwaiterBase (CPUTaskAwaiterBase&&     ) = default;
	~CPUTaskAwaiterBase()						   = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * \brief Checks if the wait has been completed already
	 * \note This function can be called even if the awaited event has been deleted already
	 * \return True if the awaiter has been completed, false otherwise
	 */
	[[nodiscard]]
	RkBool await_ready() const noexcept
	{ return CPUAwaiter<TResult>::Consumed(); }

	/**
	 * \brief Attempts a suspension by attaching the awaiter to the awaited event
	 * \return True if the suspension succeeded, false otherwise
	 */
	[[nodiscard]]
	RkBool await_suspend(std::coroutine_handle<> const in_coroutine) noexcept
	{
		m_coroutine = in_coroutine;
		return m_awaitable.TryAttach(*this);
	}

	#pragma endregion

	protected:

		CPUAwaitable<TResult> const& m_awaitable;
		std::coroutine_handle<>      m_coroutine;
};

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskAwaiter: CPUTaskAwaiterBase<TResult>
{
    /**
     * @brief Default constructor
     * @param in_awaitable Reference to the awaited event
     */
    explicit CPUTaskAwaiter(CPUAwaitable<TResult> const& in_awaitable) noexcept:
		CPUTaskAwaiterBase<TResult> {in_awaitable}
	{ this->signal = CPUSignal<TResult>(*this); }

	/// @brief Returns the result of the wait
    auto await_resume() const noexcept(!CPUTaskAwaiterBase<TResult>::has_exceptions)
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
struct CPUTaskAwaiter<TQueueHandle, RkVoid>: CPUTaskAwaiterBase<RkVoid>
{
	/**
	 * @brief Default constructor
	 * @param in_awaitable Reference to the awaited event
	 */
	explicit CPUTaskAwaiter(CPUAwaitable<RkVoid> const& in_awaitable) noexcept:
		CPUTaskAwaiterBase {in_awaitable}
	{ this->signal = CPUSignal<RkVoid>(*this); }

	/// @brief Returns the result of the wait
	static constexpr void await_resume() noexcept
	{ }

	/// @brief Called when the wait is over
	RkVoid Signal() const noexcept
	{ TQueueHandle::GetInstance().Push(this->m_coroutine); }
};

END_RUKEN_NAMESPACE
