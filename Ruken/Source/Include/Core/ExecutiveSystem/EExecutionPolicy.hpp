#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Defines the execution policy of a processing unit.
 */
enum class EExecutionPolicy
{
    /**
     * Immediate execution method means that the processing unit will
     * execute instructions as soon as possible.
     */
    Immediate,

    /**
     * Deferred execution method means that instructions sent to this processing
     * unit are recorded and will need to be manually submitted in order to start execution.
     * This is similar to the way command buffers in Vulkan works.
     */
    Deferred
};

END_RUKEN_NAMESPACE