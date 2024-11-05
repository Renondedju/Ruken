#pragma once

#include <type_traits>

#include "ExecutiveSystem/Concepts/CProcessingQueue.hpp"

BEGIN_RUKEN_NAMESPACE

template <CProcessingUnit TProcessingUnit>
struct QueueHandle;

/**
 * \brief Checks if the passed type is a processing queue handle
 */
template <typename TType>
concept CQueueHandle = requires { TType::GetInstance(); } &&
	CProcessingQueue<std::remove_reference_t<decltype(TType::GetInstance())>> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<typename TType::ProcessingUnit>, TType>;

/**
 * \brief Checks if the passed type is a submittable processing queue handle
 */
template <typename TType>
concept CSubmittableQueueHandle = requires { TType::GetInstance(); } &&
	CSubmittableProcessingQueue<std::remove_reference_t<decltype(TType::GetInstance())>> &&
	std::is_default_constructible_v<TType> &&
	std::is_base_of_v<QueueHandle<typename TType::ProcessingUnit>, TType>;


END_RUKEN_NAMESPACE