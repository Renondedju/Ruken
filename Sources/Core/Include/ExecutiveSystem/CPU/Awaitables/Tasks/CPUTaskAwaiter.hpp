#pragma once

#include "Meta/TupleHasType.hpp"
#include "ExecutiveSystem/CPU/Queues/CPUQueue.hpp"
#include "ExecutiveSystem/CPU/Awaitables/CPUAwaiter.hpp"

#include <type_traits>
#include <variant>

BEGIN_RUKEN_NAMESPACE

template <CQueueHandle TQueueHandle, CTaskResult TResult>
struct CPUTaskPromise;

template <typename TType > struct VariantHelper										      { using ValueType = TType;  using HasExceptions = std::false_type;};
template <typename TValue> struct VariantHelper<std::variant<std::exception_ptr, TValue>> { using ValueType = TValue; using HasExceptions = std::true_type; };
template <typename TValue> struct VariantHelper<std::variant<TValue, std::exception_ptr>> { using ValueType = TValue; using HasExceptions = std::true_type; };

template <CQueueHandle TQueueHandle, typename TResult>
struct CPUTaskAwaiter: CPUAwaiter
{
	CPUTaskPromise<TQueueHandle, TResult>* promise {nullptr};

	#pragma region Lifetime

	using CPUAwaiter::operator=;

	CPUTaskAwaiter& operator=(CPUTaskAwaiter const&) = default;
	CPUTaskAwaiter& operator=(CPUTaskAwaiter&&	   ) = default;

	CPUTaskAwaiter ()					   = default;
	CPUTaskAwaiter (CPUTaskAwaiter const&) = default;
	CPUTaskAwaiter (CPUTaskAwaiter&&     ) = default;
	~CPUTaskAwaiter()					   = default;

	#pragma endregion

	/// @brief Returns the result of the wait
	auto await_resume() const
	{
		if constexpr (VariantHelper<TResult>::HasExceptions::value)
			if (std::get<std::exception_ptr>(promise->result))
				std::rethrow_exception(std::get<std::exception_ptr>(promise->result));

		if constexpr (std::is_same_v<TResult, std::exception_ptr>)
			if (promise->result)
				std::rethrow_exception(promise->result);

		if constexpr (VariantHelper<TResult>::HasExceptions::value)
			return std::move(std::get<VariantHelper<TResult>::ValueType>(promise->result));

		if constexpr (!std::is_same_v<TResult, std::exception_ptr>)
			return promise->result;
	}
};

END_RUKEN_NAMESPACE
