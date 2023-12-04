#pragma once

#include <type_traits>

#include "Core/ExecutiveSystem/Concepts/ProcessingQueueType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ProcessingUnitType TProcessingUnit>
struct QueueHandle;

/**
 * \brief Checks if the passed type is a processing queue handle
 */
template <typename TType>
concept QueueHandleType = requires { TType::GetInstance(); } && 
	ProcessingQueueType<std::remove_reference_t<decltype(TType::GetInstance())>> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<typename TType::ProcessingUnit>, TType>;

/**
 * \brief Checks if the passed type is a submittable processing queue handle
 */
template <typename TType>
concept SubmittableQueueHandleType = requires { TType::GetInstance(); } && 
	SubmittableProcessingQueueType<std::remove_reference_t<decltype(TType::GetInstance())>> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<typename TType::ProcessingUnit>, TType>;


END_RUKEN_NAMESPACE