#pragma once

#include "Build/Namespace.hpp"

#include <variant>
#include <exception>
#include <type_traits>

BEGIN_RUKEN_NAMESPACE

template <typename TResult>
using SyncTaskResult = std::conditional_t<
		std::is_void_v<TResult>,
	std::exception_ptr,
	std::variant<TResult, std::exception_ptr>
>;

END_RUKEN_NAMESPACE