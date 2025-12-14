#pragma once

#include "JobSystem/Queues/JobQueue.hpp"

#include <concepts>

BEGIN_RUKEN_NAMESPACE

/// @brief Checks if the passed type is a processing queue handle
template <typename TType>
concept IsQueueHandle = requires (TType in_type) {
	{ in_type.GetInstance() } -> std::same_as<JobQueue&>;
};

END_RUKEN_NAMESPACE