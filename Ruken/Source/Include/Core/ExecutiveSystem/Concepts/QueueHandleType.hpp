#pragma once

#include <type_traits>

#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TInheriting, ProcessingQueueType TQueue>
struct QueueHandle;

/**
 * \brief Checks if the passed type is a processing queue handle
 */
template <typename TType>
concept QueueHandleType = requires { TType::queue; } && 
	ProcessingQueueType<decltype(TType::queue)> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<TType, decltype(TType::queue)>, TType>;

/**
 * \brief Checks if the passed type is a submittable processing queue handle
 */
template <typename TType>
concept SubmittableQueueHandleType = requires { TType::queue; } && 
	SubmittableProcessingQueueType<decltype(TType::queue)> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<TType, decltype(TType::queue)>, TType>;

END_RUKEN_NAMESPACE