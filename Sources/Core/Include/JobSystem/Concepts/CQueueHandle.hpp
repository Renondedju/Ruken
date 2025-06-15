#pragma once

#include <concepts>

BEGIN_RUKEN_NAMESPACE

class JobQueue;

/**
 * \brief Checks if the passed type is a processing queue handle
 */
template <typename TType>
concept CQueueHandle = requires (TType in_type) {
	{ in_type.GetInstance() } -> std::same_as<JobQueue&>;
};

END_RUKEN_NAMESPACE