#pragma once

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Defines the type of instructions executed by a processing unit
 */
enum class EInstructionType
{
    /**
     * Direct instructions means that the content of the task run by the executing system
     * are the actual instructions needed to be ran to bring the task to completion
     */
    Direct,

    /**
     * Indirect instructions means that the content of the tasks run by the executive system
     * are not the actual instruction needed to run the task to completion, instead they are an
     * indirection needed to record, or send a command to the actual physical processing unit so that the
     * designated task can start running.
     * Indirect instructions are always ran in place.
     */
    Indirect
};

END_RUKEN_NAMESPACE