#pragma once

#include "ExecutiveSystem/Concepts/CProcessingUnit.hpp"

BEGIN_RUKEN_NAMESPACE

template <CProcessingUnit TProcessingUnit>
struct ProcessingQueue;

/**
 * \brief Checks if the passed type is a processing queue
 * The passed type must:
 *  - Inherit from ProcessingQueue with itself as the first parameter type
 *
 * \tparam TType type to check
 */
template <typename TType>
concept CProcessingQueue = std::is_base_of_v<ProcessingQueue<typename TType::ProcessingUnit>, TType>;

/**
 * \brief Checks if the passed type is a submittable processing queue
 * The passed type must:
 *  - Be a processing queue type
 *  - Be owned by a recorded processing unit
 *  - Implement a static Submit method
 *
 * \tparam TType type to check
 */
template <typename TType>
concept CSubmittableProcessingQueue =
    CProcessingQueue<TType> &&
    TType::ProcessingUnit::execution_policy == EExecutionPolicy::Deferred &&
    requires () { TType::Submit(); };

END_RUKEN_NAMESPACE