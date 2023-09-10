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
concept QueueHandleType = requires { TType::instance; } && 
	ProcessingQueueType<decltype(TType::instance)> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<TType, decltype(TType::instance)>, TType>;

/**
 * \brief Checks if the passed type is a submittable processing queue handle
 */
template <typename TType>
concept SubmittableQueueHandleType = requires { TType::instance; } && 
	SubmittableProcessingQueueType<decltype(TType::instance)> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<TType, decltype(TType::instance)>, TType>;

END_RUKEN_NAMESPACE